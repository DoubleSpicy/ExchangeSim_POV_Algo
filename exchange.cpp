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
        while(true){
            zmq::message_t update;
            // keep recv order message, and try matching order, 
            // send back ACK if is POV msg
            subscriber.recv(&update); // TODO: open a thread


        }
    }

    void TradeMatchingEngine::recvMarkets(){
        
    }

    bool TradeMatchingEngine::matchOrder(){
        // TODOs:
        // 1. Fill immediately if order price is aggressive
        // 2. Queue up the order if is pasive, fill when:
        //      1. Order becomes marketable, or
        //      2. after 3 mins still is the best bid (check best bid orders, fill if time > 3mins)
        for(auto &clientOrders : currentClientOrders){
            
        }
    }

}



int main(){
    EXCHANGE::TradeMatchingEngine exchange;
    exchange.run();
}