#ifndef ALGO_ENGINE_HPP
#define ALGO_ENGINE_HPP
#include "BidAskQueue.hpp"
#include "FixParser.hpp"
#include <vector>
namespace TRADE{
    template <class T> class AlgoEngine{
        T tradeAlgo;
        MARKET::BidAskQueue orders; 
        public:
            AlgoEngine();
            bool checkMarketWithAlgo();
    };

    class POVAlgo{
        public:
            bool produceOrders(std::vector<FIX::order> &orders); // if algo engine wanna produce orders, put them into the vector, and return true
    };
}
#endif