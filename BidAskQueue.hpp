#ifndef BID_ASK_QUEUE_HPP
#define BID_ASK_QUEUE_HPP
#include <queue>
#include "FixParser.hpp"

namespace MARKET{
    class BidAskQueue{
        // std::priority_queue<FIX::order, std::vector<FIX::order>,
        // compareFixPriceGreater> bidQueue;
        // std::priority_queue<FIX::order, std::vector<FIX::order>, 
        // compareFixPriceSmaller> askQueue;
        std::deque<FIX::order> bidQueue;
        std::deque<FIX::order> askQueue;
        public:
            BidAskQueue();
            void insertBid(const FIX::order & ord);
            void insertAsk(const FIX::order & ord);
            void clearBid();
            void clearAsk();
            FIX::order popBid();
            FIX::order popAsk();
            bool fillOrder(FIX::order & filledBid, FIX::order & filledAsk);
    };
};


#endif