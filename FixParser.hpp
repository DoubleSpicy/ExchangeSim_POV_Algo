#ifndef FIX_PARSER_HPP
#define FIX_PARSER_HPP
#include <string>
#include <sstream>
#include <iostream>

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
            long SendingTime;
            std::string Side;
            float POVTargetPercentage;
            bool isFromClient;
            order();
            order(const std::string &MsgType,
                const std::string &OrderID, 
                const float &OrderQty, 
                const std::string &OrdType,
                const float &Price,
                const long &SendingTime,
                const std::string &Side, 
                const float &POVTargetPercentage,
                const bool &isFromClient);
            order(const std::string &msg);
            std::string convertToString() override;
            inline bool compareID(const order & other) const;
            inline bool operator> (const order & other) const;
            inline bool operator< (const order & other) const;
            inline bool operator>= (const order & other) const;
            inline bool operator<= (const order & other) const;
            
            // ~order();
    };

};

















#endif