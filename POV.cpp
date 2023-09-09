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
    double & cumulativeTradedVol, double & position, std::string & orderID, std::string & senderCompID, std::deque<FIX::order> & openOrders){
        bool res = false;
        if(targetQuantity <= 0) return res;
        // implement POV algo here.
        // dummy firing algo
        // 
        // for(auto &bidQuote: bidAskQ.bidQueue){
        //     float volToBuy = std::min(bidQuote.OrderQty*targetPercentage, targetQuantity);
        //     float price = bidQuote.Price;
        //     auto millisecondsUTC = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        //     FIX::order newOrder("0", orderID, volToBuy, "0", price, senderCompID, millisecondsUTC, "BUY", targetPercentage);
        //     orderID = std::to_string(stol(orderID) + 1);
        //     newOrders.push_back(newOrder);
        // }
        // ===============

        // 1 level implementation
        float sum = 0;
        for(auto &ord: openOrders){
            sum += ord.Price;
        }
        float curPercent = sum / cumulativeTradedVol;
        if(curPercent > 1.2){
            std::deque<FIX::order> temp;
            for(auto &ord : openOrders){
                FIX::order newCancel("1", openOrders.back().OrderID, openOrders.back().OrderQty, openOrders.back().OrdType, openOrders.back().Price, openOrders.back().SenderCompID
                , openOrders.back().SendingTime, openOrders.back().Side, openOrders.back().POVTargetPercentage);
                targetQuantity += openOrders.back().OrderQty;
                temp.push_back(newCancel);
            }
            return true;
        }

        float volTarget;
        float price;
        if(curPercent < 0.8){
            volTarget = std::min(bidAskQ.askQueue.front().OrderQty*targetPercentage, targetQuantity);
            price = bidAskQ.askQueue.front().Price;
        }
        else{
            volTarget = std::min(bidAskQ.bidQueue.front().OrderQty*targetPercentage, targetQuantity);
            price = bidAskQ.bidQueue.front().Price;
        }
        float volToBuy = volTarget;
        for(auto it = openOrders.rbegin(); it != openOrders.rend(); it++){
            volToBuy -= it->OrderQty;
        }

        std::deque<FIX::order> temp;
        while(volToBuy < 0){
            FIX::order newCancel("1", openOrders.back().OrderID, openOrders.back().OrderQty, openOrders.back().OrdType, openOrders.back().Price, openOrders.back().SenderCompID
            , openOrders.back().SendingTime, openOrders.back().Side, openOrders.back().POVTargetPercentage);
            volToBuy += openOrders.back().OrderQty;
            targetQuantity += openOrders.back().OrderQty;
            temp.push_back(newCancel);
            openOrders.pop_back();
        }
        for(auto &e: temp){
            newOrders.push_back(e);
        }
        
        auto millisecondsUTC = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        FIX::order newOrder("0", orderID, volToBuy, "0", price, senderCompID, millisecondsUTC, "BUY", targetPercentage);
        newOrders.push_back(newOrder);
        orderID = std::to_string(stol(orderID) + 1);




        // float completedTarget = (float)(position / cumulativeTradedVol);
        // if(completedTarget > 1.2){
        //     // halt
        // }
        // else if (completedTarget < 0.8){
        //     // aggressive buy

        // }
        // else{
        //     // Passive Posting
        //     for(auto &bidQuote: bidAskQ.bidQueue){
        //         float volToBuy = std::min(bidQuote.OrderQty*targetPercentage, targetQuantity);
        //         float price = bidQuote.Price;
        //         FIX::order marketQuote("0", "0", 1234, "0", 7654, "0", 0, "BID", 0);
        //         auto millisecondsUTC = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        //         FIX::order newOrder("0", orderID, volToBuy, "0", price, senderCompID, millisecondsUTC, "BUY", targetPercentage);
        //         orderID = std::to_string(stol(orderID) + 1);
        //     }
        // }

        return res;
    }
}