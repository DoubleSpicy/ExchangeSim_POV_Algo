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

        // TODO: FORCE FILL AFTER 3 MIN
        // if(clientOrders.size() > 0){
        //     if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - client).count() )
        // }

        for(auto clientIterator = clientOrders.begin(); clientIterator != clientOrders.end();){
            for(auto askIterator = askQueue.begin(); askIterator != askQueue.end();){
                if(almost_equal(clientIterator->Price, askIterator->Price, 5)){
                    res = true;
                    float amountFilled = std::min(clientIterator->OrderQty, askIterator->OrderQty);
                    clientIterator->OrderQty -= amountFilled;
                    askIterator->OrderQty -= amountFilled;
                    // create ACK FILLED message
                    FIX::ACK ACKmessage(clientIterator->SenderCompID, "3", clientIterator->OrderID, amountFilled);
                    filledOrders.push_back(ACKmessage);
                    if(almost_equal(askIterator->OrderQty, (float)0, 5)){
                        askIterator = askQueue.erase(askIterator);
                    }
                    else{
                        askIterator++;
                    }
                    break;
                }
            }
            if(almost_equal(clientIterator->OrderQty, (float)0, 5)){
                clientIterator = clientOrders.erase(clientIterator);
            }
            else{
                clientIterator++;
            }
        }

        return res;
    }

}