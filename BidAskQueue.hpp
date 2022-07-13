#ifndef BID_ASK_QUEUE_HPP
#define BID_ASK_QUEUE_HPP
#include <queue>
#include "FixParser.hpp"
#include <algorithm>
#include <functional>
#include <cmath>
#include <chrono>

namespace MARKET{
    class BidAskQueue{
        // std::priority_queue<FIX::order, std::vector<FIX::order>,
        // compareFixPriceGreater> bidQueue;
        // std::priority_queue<FIX::order, std::vector<FIX::order>, 
        // compareFixPriceSmaller> askQueue;
        public:
            BidAskQueue();
            std::deque<FIX::order> bidQueue;
            std::deque<FIX::order> askQueue;
            std::deque<FIX::order> clientOrders;
            void insertBid(const FIX::order & ord);
            void insertAsk(const FIX::order & ord);
            void clearBid();
            void clearAsk();
            FIX::order popBid();
            FIX::order popAsk();
            bool fillOrders(std::vector<FIX::ACK> & filledOrders);
            bool tryFillAggressiveOrder(FIX::order & ord, std::vector<FIX::ACK> & filledOrders);
            bool tryFill3MinsOrder(FIX::order & ord, std::vector<FIX::ACK> & filledOrders);
            void addingQuotesIntoQueues(const std::string & updt);
    };
};


#endif