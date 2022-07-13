#ifndef POV_HPP
#define POV_HPP

#include <vector>
#include "FixParser.hpp"
#include "BidAskQueue.hpp"
namespace ALGO{
    class POVAlgo{
        public:
            bool produceOrders(std::vector<FIX::order> &newOrders, MARKET::BidAskQueue bidAskQ); // if algo engine wanna produce orders, put them into the vector, and return true
    };
}





#endif