#include "BidAskQueue.hpp"
 
template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp)
{
    // https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * ulp
        // unless the result is subnormal
        || std::fabs(x-y) < std::numeric_limits<T>::min();
}

namespace MARKET{
    // double epsilon = 0.00001;

    BidAskQueue::BidAskQueue(){

    }

    void BidAskQueue::insertBid(const FIX::order & ord){
        // auto it = std::upper_bound(bidQueue.begin(), bidQueue.end(), ord, std::greater<FIX::order>());
        bidQueue.push_front(ord);
    }

    void BidAskQueue::insertAsk(const FIX::order & ord){
        // auto it = std::upper_bound(askQueue.begin(), askQueue.end(), ord);
        askQueue.push_back(ord);
    }

    void BidAskQueue::clearBid(){
        bidQueue.clear();
    }

    void BidAskQueue::clearAsk(){
        askQueue.clear();
    }

    FIX::order BidAskQueue::popBid(){
        FIX::order res = std::move(bidQueue.front());
        bidQueue.pop_front();
        return res;
    }

    FIX::order BidAskQueue::popAsk(){
        FIX::order res = std::move(askQueue.front());
        askQueue.pop_front();
        return res;
    }

    bool BidAskQueue::fillOrders(std::vector<FIX::ACK> & filledOrders){
        bool res = false;
        std::cout << "cur qty: " << clientOrders[0].OrderQty << ", askQueueSize: " << askQueue.size() << ", clientOrderSize: " << clientOrders.size() << std::endl;
        for(auto clientIterator = clientOrders.begin(); clientIterator != clientOrders.end();){
            for(auto askIterator = askQueue.begin(); askIterator != askQueue.end(); askIterator++){
                // std::cout << "Comparing prices: " << clientIterator->Price << " " << askIterator->Price << std::endl;
                if(almost_equal(clientIterator->Price, askIterator->Price, 2)){
                    res = true;
                    float amountFilled = std::min(clientIterator->OrderQty, askIterator->OrderQty);
                    clientIterator->OrderQty -= amountFilled;
                    // askIterator->OrderQty -= amountFilled;
                    std::cout << "Filled :" << amountFilled << " ,order: " << clientIterator->to_string() << std::endl;
                    // create ACK FILLED message
                    FIX::ACK ACKmessage(clientIterator->SenderCompID, "3", clientIterator->OrderID, amountFilled);
                    filledOrders.push_back(ACKmessage);
                }
                // if(almost_equal(askIterator->OrderQty, (float)0, 5)){
                //     askIterator = askQueue.erase(askIterator);
                // }
                // else{
                //     askIterator++;
                // }
            }
            // std::cout << "cur qty: " << clientIterator->OrderQty << std::endl;
            if(almost_equal(clientIterator->OrderQty, (float)0, 2)){
                clientIterator = clientOrders.erase(clientIterator);
            }
            else{
                clientIterator++;
            }
        }

        return res;
    }

    void BidAskQueue::addingQuotesIntoQueues(const std::string & updt){
            bidQueue.clear();
            askQueue.clear();
            // parsing the quotes
            std::string parsedStr;
            std::stringstream semiColonParser(updt);
            int idx = 0;
            while(std::getline(semiColonParser, parsedStr, ';')){
                if(idx == 2){
                    // bid quotes
                    std::vector<FIX::order> bidQuotes = FIX::parseQuotes(parsedStr);
                    for(auto &quote : bidQuotes){
                        insertBid(quote);
                        std::cout << "BID PARSER: " << quote.to_string() << std::endl;
                    }
                }
                if(idx == 3){
                    // ask quotes
                    std::vector<FIX::order> askQuotes = FIX::parseQuotes(parsedStr);
                    for(auto &quote : askQuotes){
                        insertAsk(quote);
                        std::cout << "ASK PARSER: " << quote.to_string() << std::endl;
                    }
                }
                idx++;
                
            }
    }

    bool BidAskQueue::cancelOrder(const FIX::order & cancelRequest, std::vector<FIX::ACK> cancelledOrders){
        bool res = false; // successful cancellation => true
        for(auto it = clientOrders.begin(); it != clientOrders.end(); it++){
            if(cancelRequest.OrderID == it->OrderID){
                res = true;
                FIX::ACK ackCancelMsg(cancelRequest.SenderCompID, "5", cancelRequest.OrderID, -1);
                cancelledOrders.push_back(ackCancelMsg);
                clientOrders.erase(it);
                break;
            }
        }
        return res;
    }


    bool BidAskQueue::tryFill3MinsOrder(std::vector<FIX::ACK> & filledOrders){
        bool res = false;
        for(auto it = clientOrders.begin(); it != clientOrders.end(); ){
            auto msUnixTimeNow = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
            std::cout << "msUnixTimeNow: " << msUnixTimeNow << "\nSendingTime: " << it->SendingTime << std::endl;
            if(msUnixTimeNow - it->SendingTime >= 180000){ // 180000
                res = true;
                FIX::ACK ackFillMsg(it->SenderCompID, "5", it->OrderID, -1);
                filledOrders.push_back(ackFillMsg);
                it = clientOrders.erase(it);
                std::cout << "erased!\n";
            }
            else{
                break;
            }
        }

        return res;
    }

}