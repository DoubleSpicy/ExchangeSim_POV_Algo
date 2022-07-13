#ifndef BID_ASK_QUEUE_H
#define BID_ASK_QUEUE_H
#include <queue>
#include "FixParser.h"

namespace MARKET{
    struct compareFixPriceGreater{
        bool operator()(const FIX::order & a, const FIX::order & b){
            return (a.Price != b.Price ) ? (a.Price > b.Price) : (a.OrderID < b.OrderID);
        }
    };

    struct compareFixPriceSmaller{
        bool operator()(const FIX::order & a, const FIX::order & b){
            return (a.Price != b.Price ) ? (a.Price < b.Price) : (a.OrderID < b.OrderID);
        }
    };


    class BidAskQueue{
        std::priority_queue<FIX::order, std::vector<FIX::order>,
        compareFixPriceGreater> bidQueue;
        std::priority_queue<FIX::order, std::vector<FIX::order>, 
        compareFixPriceSmaller> askQueue;
        public:
            BidAskQueue();
            void insertBid(const FIX::order & ord);
            void insertAsk(const FIX::order & ord);
            void clearBid();
            void clearAsk();
            FIX::order popBid();
            FIX::order popAsk();
            // void fillOrder();
    };
};


#endif