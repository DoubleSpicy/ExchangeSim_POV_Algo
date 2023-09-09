#ifndef ALGO_ENGINE_HPP
#define ALGO_ENGINE_HPP
#include "BidAskQueue.hpp"
#include "FixParser.hpp"
#include <vector>
#include <unordered_map>

namespace TRADE{
    template <class T> class AlgoEngine{
        double cumulativeMarketVol = 0, position = 0;
        std::string orderID = "0", senderCompID = "0";
        // std::unordered_map<float, std::pair<double, std::deque<FIX::order>>> & openOrders
        std::deque<FIX::order> openOrders;
        public:
            T tradeAlgo;
            MARKET::BidAskQueue bidAskQ; 
            AlgoEngine(const T & tradeAlgo);
            // AlgoEngine();
            void run();
            // tradeAlgo.template produceOrders<bool>(std::vector<FIX::order> &orders);
    };


}
#endif