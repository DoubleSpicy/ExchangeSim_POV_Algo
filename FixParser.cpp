// library to parse fix messages
// https://www.onixs.biz/fix-dictionary/4.4/fields_by_tag.html
#include "FixParser.hpp"

namespace FIX {
    float roundingOffFloat(const float & val, const int & precision = 10000){
        return round(val * precision) / precision;
    }

    message::message(){
        // std::cout << "TEST\n";
    }



    order::order(const std::string &MsgType,
                const std::string &OrderID, 
                const float &OrderQty, 
                const std::string &OrdType,
                const float &Price,
                const std::string &SenderCompID,
                const long &SendingTime,
                const std::string &Side, 
                const float &POVTargetPercentage):
                MsgType(MsgType),
                OrderID(OrderID),
                OrderQty(OrderQty),
                OrdType(OrdType),
                Price(Price),
                SenderCompID(SenderCompID),
                SendingTime(SendingTime),
                Side(Side),
                POVTargetPercentage(POVTargetPercentage)
                {};

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
                case 49:
                    SenderCompID = body;
                case 52:
                    SendingTime = stoll(body);
                case 54:
                    Side = body;
                    break;
                case 6404:
                    POVTargetPercentage = stof(body);
                    break;
            }
        }
    }

    template < typename Type > std::string to_str (const Type & t)
    {
    std::ostringstream os;
    os << t;
    return os.str ();
    }

    std::string order::to_string(){
    std::string res = "35=" + MsgType + ";49=" + SenderCompID + ";37=" + OrderID + ";38=" + to_str(OrderQty)
        + ";40=" + OrdType + ";44=" + to_str(Price) + ";52=" + to_str(SendingTime) + ";54=" + Side
        + ";6404=" + to_str(POVTargetPercentage);
    return res;
    }


    // inline bool FIX::order::compareID(const FIX::order & b) const {
    //     return OrderID < b.OrderID;
        
    // }

    // // for comparing order prices
    // inline bool FIX::order::operator> (const FIX::order & other) const {
    //     return (Price != other.Price) ? (Price > other.Price) : (compareID(other));
    // }

    // inline bool FIX::order::operator< (const FIX::order & other) const {
    //     return (Price != other.Price) ? (Price < other.Price) : (compareID(other));
    // }

    // inline bool FIX::order::operator>= (const FIX::order & other) const {
    //     return (Price != other.Price) ? (Price >= other.Price) : (compareID(other));
    // }

    // inline bool FIX::order::operator<= (const FIX::order & other) const {
    //     return (Price != other.Price) ? (Price <= other.Price) : (compareID(other));
    // }

    ACK::ACK(const std::string & TargetCompID,
                const std::string & MsgType,
                const std::string OrderID,
                const float & OrderQty):
                TargetCompID(TargetCompID),
                MsgType(MsgType),
                OrderID(OrderID),
                OrderQty(OrderQty){};

    std::string ACK::to_string(){
        return "35=" + MsgType + ";56=" + TargetCompID + ";37=" + OrderID + ";38=" + std::to_string(OrderQty);
    }

    std::vector<FIX::order> parseQuotes(const std::string & marketData){
        // input a one-sided quote message, return the parsed quotes of FIX objects.
        // e.g. 53.000 10500 52.950 1000 52.900 2000
        // becomes [53.000, 10500], [52.950, 1000], [52.900, 2000]
        std::vector<FIX::order> res;
        std::vector<std::string> quote;
        std::stringstream parser(marketData);
        std::string temp;
        while(std::getline(parser, temp, ' ')){
            quote.push_back(temp);
            if(quote.size() == 2){
                // std::cout << "PARSING: " << quote[1] << "=>" << roundingOffFloat(stof(quote[1])) << ", " << quote[0] << "=>" << roundingOffFloat(stof(quote[0])) << std::endl;
                FIX::order marketQuote("0", "-1", roundingOffFloat(stof(quote[1])), "-1", roundingOffFloat(stof(quote[0])), "-1", 
                std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count(), "BID", 0);
                res.push_back(marketQuote);
                quote.clear();
            }
        }
        return res;
    }

    void sendAllMessages(std::vector<FIX::order> & filledOrders, zmq::socket_t & publisher)
    {
        zmq::message_t msg(1);
        for(auto &ackMessages: filledOrders){
            std::string data = ackMessages.to_string();
            msg.rebuild(data.size());
            memcpy(msg.data(), data.data(), data.size());
            std::cout << "sent: " << data << std::endl;
            publisher.send(msg);
        }
    }

    void sendAllMessages(std::vector<FIX::ACK> & ACKs, zmq::socket_t & publisher){
        zmq::message_t msg(1);
        for(auto &ackMessages: ACKs){
            std::string data = ackMessages.to_string();
            msg.rebuild(data.size());
            memcpy(msg.data(), data.data(), data.size());
            std::cout << "sent: " << data << std::endl;
            publisher.send(msg);
        }
    }
}


// FIX::order::~order(){};

