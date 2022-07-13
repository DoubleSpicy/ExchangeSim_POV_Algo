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
        std::cout << "running...\n";
        while(true){
            zmq::message_t update;
            // keep recv order message, and try matching order, 
            // send back ACK if is POV msg
            subscriber.recv(&update); // TODO: open a thread
            std::string updt = std::string(static_cast<char*>(update.data()), update.size());
            std::cout << "Received Msg: " << updt << ":" <<update.size() << std::endl;
            
            
            
        }
    }

}



int main(){
    EXCHANGE::TradeMatchingEngine exchange;
    exchange.run();
}