#ifndef ALGO_ENGINE_HPP
#define ALGO_ENGINE_HPP
#include "BidAskQueue.hpp"
#include "FixParser.hpp"
#include <vector>
namespace TRADE{
    template <class T> class AlgoEngine{
        T tradeAlgo;
        MARKET::BidAskQueue bidAskQ; 
        public:
            AlgoEngine(const T & tradeAlgo);
            void run();
            // tradeAlgo.template produceOrders<bool>(std::vector<FIX::order> &orders);
    };


}
#endif