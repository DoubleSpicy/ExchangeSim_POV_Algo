#ifndef FIX_PARSER_HPP
#define FIX_PARSER_HPP
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include <type_traits>
#include <algorithm>
#include <fstream>
#include "zmq.hpp"

namespace FIX{
    class message{
        public:
            message();
            // ~message();
            virtual std::string to_string() = 0;
            // virtual ~message();
    };
    class order: message{
        public:
            std::string MsgType;
            std::string OrderID;
            float OrderQty;
            std::string OrdType;
            float Price;
            std::string SenderCompID;
            long SendingTime;
            std::string Side;
            float POVTargetPercentage;

            order();

            order(const std::string &MsgType,
                const std::string &OrderID, 
                const float &OrderQty, 
                const std::string &OrdType,
                const float &Price,
                const std::string &SenderCompID,
                const long &SendingTime,
                const std::string &Side, 
                const float &POVTargetPercentage);

            order(const std::string &msg);

            std::string to_string() override;

            // inline bool compareID(const order & other) const;
            // inline bool operator> (const order & other) const;
            // inline bool operator< (const order & other) const;
            // inline bool operator>= (const order & other) const;
            // inline bool operator<= (const order & other) const;
            
            // ~order();
    };
    
    class ACK: message{
        // conveying ACK ORDER or ACK FILL or ACK CANCELLED message
        public:
            std::string TargetCompID; // to send to specific client
            std::string MsgType; // ACK ORDER / ACK FILL / ACK CANCELLED
            std::string OrderID; // original order id
            float OrderQty; // quantity invloved
            float Price;
            ACK(const std::string & TargetCompID,
                const std::string & MsgType,
                const std::string OrderID,
                const float & OrderQty,
                const float & Price);
            ACK(const std::string & msg);
            std::string to_string() override;
    };

    std::vector<FIX::order> parseQuotes(const std::string & marketData);

    // template <typename T, typename A>
    void sendAllMessages(std::vector<FIX::ACK> & filledOrders, zmq::socket_t & publisher, std::ofstream & log, float & cumulativeQuantity);
    void sendAllMessages(std::vector<FIX::order> & filledOrders, zmq::socket_t & publisher, std::ofstream & log);
};

















#endif