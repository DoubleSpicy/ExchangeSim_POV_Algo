#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP

#include "BidAskQueue.hpp"
#include "zmq.hpp"
#include <chrono>
#include <vector>

namespace EXCHANGE{
    class TradeMatchingEngine{
        MARKET::BidAskQueue BidAsk;
        float cumulativeQty = 0;
        // std::vector<FIX::order> currentClientOrders;
        
        
        public:
            void run(); // non-stop matching
    };
}
#endif