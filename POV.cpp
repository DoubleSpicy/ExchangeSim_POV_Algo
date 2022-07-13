#include "POV.hpp"


namespace ALGO{
    void POVAlgo::setMinRatio(const float & val){
        minRatio = std::move(val);
    }

    void POVAlgo::setMaxRatio(const float & val){
        maxRatio = std::move(val);
    }

    void POVAlgo::setTargetPercentage(const float & val){
        targetPercentage = std::move(val);
    }

    void POVAlgo::setTargetQuantity(const float & val){
        targetQuantity= std::move(val);
    }


    bool POVAlgo::execute(std::vector<FIX::order> &newOrders, MARKET::BidAskQueue &bidAskQ,
    double & cumulativeTradedVol, double & position, std::string & orderID, std::string & senderCompID){
        // implement POV algo here.
        bool res = false;
        float completedTarget = (float)(position / cumulativeTradedVol);
        if(completedTarget > 1.2){
            // halt
        }
        else if (completedTarget < 0.8){
            // aggressive buy

        }
        else{
            // Passive Posting
            for(auto &bidQuote: bidAskQ.bidQueue){
                float volToBuy = std::min(bidQuote.OrderQty*targetPercentage, targetQuantity);
                float price = bidQuote.Price;
                FIX::order marketQuote("0", "0", 1234, "0", 7654, "0", 0, "BID", 0);
                auto millisecondsUTC = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                FIX::order newOrder("0", orderID, volToBuy, "0", price, senderCompID, std::to_string(millisecondsUTC), "BUY", targetPercentage);
                orderID = std::to_string(stol(orderID) + 1);
            }
        }

        return res;
    }
}