// library to parse fix messages
// https://www.onixs.biz/fix-dictionary/4.4/fields_by_tag.html
#include "FixParser.hpp"


namespace FIX {
    message::message(){
        // std::cout << "TEST\n";
    }

    order::order(const std::string &MsgType,
    const std::string &OrderID, 
    const float &OrderQty, 
    const std::string &OrdType,
    const float &Price,
    const long &SendingTime,
    const std::string &Side, 
    const float &POVTargetPercentage,
    const bool &isFromClient);
    MsgType(MsgType),
    OrderID(OrderID),
    OrderQty(OrderQty),
    OrdType(OrdType),
    Price(Price),
    SendingTime(SendingTime),
    Side(Side),
    POVTargetPercentage(POVTargetPercentage),
    isFromClient(isFromClient){};

    order::order(){

    }
    
    order::order(const std::string &msg){
        // parse string FIX to FIX obj
        std::stringstream parser(msg);
        std::string segment;
        while(std::getline(parser, segment, ';')){
            int type = stoi(segment.substr(0, segment.find('=')));
            std::string body = segment.substr(segment.find('=')+1, segment.size()-1);
            switch(type){
                case 35:
                    MsgType = body;
                    break;
                case 37:
                    OrderID = body;
                    break;
                case 38:
                    OrderQty = stof(body);
                    break;
                case 40:
                    OrdType = body;
                    break;
                case 44:
                    Price = stof(body);
                    break;
                case 52:
                    SendingTime = stoll(body);
                case 54:
                    Side = body;
                    break;
                case 6404:
                    POVTargetPercentage = stof(body);
                    break;
                case 9999:
                    isFromClient = stoi(body);
            }
        }
    }

    std::string order::convertToString(){
    std::string res = "35=" + MsgType + ";37=" + OrderID + ";38=" + std::to_string(OrderQty)
        + ";40=" + OrdType + ";44=" + std::to_string(Price) + ";54=" + Side
        + ";6404=" + std::to_string(POVTargetPercentage);
    return res;
    }


    inline bool FIX::order::compareID(const FIX::order & b) const {
        return OrderID < b.OrderID;
        
    }

    // for comparing order prices
    inline bool FIX::order::operator> (const FIX::order & other) const {
        return (Price != other.Price) ? (Price > other.Price) : (compareID(other));
    }

    inline bool FIX::order::operator< (const FIX::order & other) const {
        return (Price != other.Price) ? (Price < other.Price) : (compareID(other));
    }

    inline bool FIX::order::operator>= (const FIX::order & other) const {
        return (Price != other.Price) ? (Price >= other.Price) : (compareID(other));
    }

    inline bool FIX::order::operator<= (const FIX::order & other) const {
        return (Price != other.Price) ? (Price <= other.Price) : (compareID(other));
    }
}


// FIX::order::~order(){};

