#ifndef POV_HPP
#define POV_HPP

#include <vector>
#include "FixParser.hpp"
#include "BidAskQueue.hpp"
#include <chrono>
#include <string>

namespace ALGO{
    class POVAlgo{
        float minRatio = 0.8, maxRatio = 1.2, targetPercentage = 0, targetQuantity = 0;
        public:
            bool execute(std::vector<FIX::order> & newOrders, MARKET::BidAskQueue & bidAskQ,
            double & cumulativeTradedVol, double & position, std::string & orderID, std::string & senderCompID); // if algo engine wanna produce orders, put them into the vector, and return true
            void addCumulativeMarketVol(const float & amount);
            void setMinRatio(const float & val);
            void setMaxRatio(const float & val);
            void setTargetPercentage(const float & val);
            void setTargetQuantity(const float & val);
    };
}





#endif