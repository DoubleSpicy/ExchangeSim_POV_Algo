#ifndef FIX_PARSER_H
#define FIX_PARSER_H
#include <string>
#include <sstream>
#include <iostream>
// #include "FixParser.cpp"
namespace FIX{
    class message{
        public:
            message();
            // ~message();
            virtual std::string convertToString() = 0;
            // virtual ~message();
    };
    class order: message{
        public:
            std::string MsgType;
            std::string OrderID;
            float OrderQty;
            std::string OrdType;
            float Price;
            std::string Side;
            float POVTargetPercentage;
            order(const std::string &MsgType,
                const std::string &OrderID, 
                const float &OrderQty, 
                const std::string &OrdType,
                const float &Price,
                const std::string &Side, 
                const float &POVTargetPercentage);
            order(const std::string &msg);
            std::string convertToString();
            
            // ~order();
    };
};

















#endif