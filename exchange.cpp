// exchange mockup
#include "exchange.hpp"
#include <mutex>
// TODO: split m
namespace EXCHANGE{
    void TradeMatchingEngine::run(){
        zmq::context_t context(1);
        zmq::socket_t subscriber (context, ZMQ_SUB);
        subscriber.connect("tcp://127.0.0.1:5556");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "Q", 1); // subscribe to Q-typed market data
        std::cout << "running...market-sub\n";


        zmq::context_t clientContext(1);
        zmq::socket_t orderSubscriber (context, ZMQ_SUB);
        orderSubscriber.bind("tcp://127.0.0.1:5557");
        orderSubscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0); // subscribe to Q-typed market data
        std::cout << "running...order-sub\n";



        while(true){
            zmq::message_t update;
            // keep recv order message, and try matching order, 
            // send back ACK if is POV msg
            subscriber.recv(&update); // TODO: open a thread
            std::string updt = std::string(static_cast<char*>(update.data()), update.size());
            std::cout << "Received Msg: " << updt << ":" <<update.size() << std::endl;
            
            std::string parsedStr;
            std::stringstream semiColonParser(updt);
            int idx = 0;
            while(std::getline(semiColonParser, parsedStr, ';')){
                if(idx == 2){
                    // bid quotes
                    std::vector<FIX::order> bidQuotes = FIX::parseQuotes(parsedStr);
                    for(auto &quote : bidQuotes){
                        BidAsk.insertBid(quote);
                        std::cout << "PARSER: " << quote.to_string() << std::endl;
                    }
                }
                if(idx == 3){
                    // ask quotes
                    std::vector<FIX::order> askQuotes = FIX::parseQuotes(parsedStr);
                    for(auto &quote : askQuotes){
                        BidAsk.insertAsk(quote);
                        std::cout << "PARSER: " << quote.to_string() << std::endl;
                    }
                }
                idx++;
                
            }

            // zmq::message_t update;
            // keep recv order message, and try matching order, 
            // send back ACK if is POV msg
            std::cout <<"waiting...\n";
            orderSubscriber.recv(&update); // TODO: open a thread
            updt = std::string(static_cast<char*>(update.data()), update.size());
            std::cout << "Received Msg: " << updt << ":" <<update.size() << std::endl;


            
            
        }
    }

}



int main(){
    EXCHANGE::TradeMatchingEngine exchange;
    exchange.run();
}