#include "BidAskQueue.hpp"

namespace MARKET{
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

        // TODO: FORCE FILL AFTER 3 MIN
        // if(clientOrders.size() > 0){
        //     if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - client).count() )
        // }

        for(auto clientIterator = clientOrders.begin(); clientIterator != clientOrders.end();){
            for(auto askIterator = askQueue.begin(); askIterator != askQueue.end();){
                if(abs(clientIterator->Price - askIterator->Price) < 0.00005){
                    res = true;
                    float amountFilled = std::min(clientIterator->OrderQty, askIterator->OrderQty);
                    clientIterator->OrderQty -= amountFilled;
                    askIterator->OrderQty -= amountFilled;
                    // create ACK FILLED message
                    FIX::ACK ACKmessage(clientIterator->SenderCompID, "3", clientIterator->OrderID, amountFilled);
                    filledOrders.push_back(ACKmessage);
                    if(askIterator->OrderQty == 0){
                        askIterator = askQueue.erase(askIterator);
                    }
                    else{
                        askIterator++;
                    }
                    break;
                }
            }
            if(abs(clientIterator->OrderQty) < 0.00005 && abs(clientIterator->OrderQty) > -0.00005){
                clientIterator = clientOrders.erase(clientIterator);
            }
            else{
                clientIterator++;
            }
        }

        return res;
    }

}