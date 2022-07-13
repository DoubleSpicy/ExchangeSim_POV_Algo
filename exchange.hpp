#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP

#include "BidAskQueue.hpp"
#include "zmq.hpp"
#include <chrono>
#include <vector>

namespace EXCHANGE{
    class TradeMatchingEngine{
        MARKET::BidAskQueue BidAsk;
        std::vector<FIX::order> currentClientOrders;
        
        
        public:
            void run(); // non-stop matching
            void recvOrders(); // recv orders from algo
            void recvMarkets(); // recv market data
            void sendACK(); // send ack msg to algo engine
            bool matchOrder(); // try fill order from algo engine
    };
}
#endif