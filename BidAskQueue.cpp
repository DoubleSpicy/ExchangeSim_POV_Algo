#include "BidAskQueue.hpp"
#include <algorithm>
#include <functional>

namespace MARKET{
    BidAskQueue::BidAskQueue(){

    }

    void BidAskQueue::insertBid(const FIX::order & ord){
        auto it = std::upper_bound(bidQueue.begin(), bidQueue.end(), ord, std::greater<FIX::order>());
        bidQueue.insert(it, ord);
    }

    void BidAskQueue::insertAsk(const FIX::order & ord){
        auto it = std::upper_bound(askQueue.begin(), askQueue.end(), ord);
        askQueue.insert(it, ord);
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

    bool BidAskQueue::fillOrder(FIX::order & bidFilled, FIX::order & askFilled){
        if(askQueue.front() <= bidQueue.front()){
            bidFilled = std::move(bidQueue.front());
            askFilled = std::move(askQueue.front());
            bidQueue.pop_front();
            askQueue.pop_front();
            return true;
        }
        return false;
    }

}