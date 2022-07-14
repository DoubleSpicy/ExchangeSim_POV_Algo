// algo engine
#include "algoEngine.hpp"
#include "zmq.hpp"
#include "POV.hpp"
#include <functional>

namespace TRADE{
    template <class T> 
    AlgoEngine<T>::AlgoEngine(const T & algorithm): tradeAlgo(algorithm){};

    template <class T> 
    void AlgoEngine<T>::run(){
        zmq::context_t context (1);
        zmq::socket_t subscriber (context, ZMQ_SUB); // subscribe to market data
        subscriber.connect("tcp://127.0.0.1:5556");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

        zmq::context_t contextPub (1);
        zmq::socket_t publisher(contextPub, ZMQ_PUB); // for sending orders to exchange.
        publisher.connect("tcp://127.0.0.1:5557"); // can be remapped using docker.
        zmq::message_t msg(1);
        std::string data = "abc123";
        std::stringstream ss;
        std::cout << "firing data\n";
        while(true){
            // get newest market data
            // SYSTEM BLOCKING CALL
            zmq::message_t update;
            subscriber.recv (&update);
            std::string updt = std::string(static_cast<char*>(update.data()), update.size());
            std::cout << "Received Msg: " << updt << ":" <<update.size() << std::endl;
            // END

            bidAskQ.addingQuotesIntoQueues(updt);

            std::vector<FIX::order> newOrders;
            tradeAlgo.execute(newOrders, bidAskQ, cumulativeMarketVol, position, orderID, senderCompID); // execute the strategy
            FIX::sendAllMessages(newOrders, publisher);
        }
    }
    
}

// class dummyAlgo {
//     public:
//         dummyAlgo(){
//             std::cout << "hi\n";
//         }
// };

int main(){
    ALGO::POVAlgo algo;
    TRADE::AlgoEngine<ALGO::POVAlgo> instance(algo);
    // std::invoke(TRADE::AlgoEngine<dummyAlgo>::run(), instance);
    instance.run();
}