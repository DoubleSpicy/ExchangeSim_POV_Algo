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
        std::ofstream algoEngineLog("log.txt");

        zmq::context_t context (1);
        zmq::socket_t subscriber (context, ZMQ_SUB); // subscribe to market data
        subscriber.connect("tcp://127.0.0.1:5556");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

        zmq::context_t contextPub (1);
        zmq::socket_t publisher(contextPub, ZMQ_PUB); // for sending orders to exchange.
        publisher.connect("tcp://127.0.0.1:5557"); // can be remapped using docker.
        zmq::message_t msg(1);
        std::cout << "firing data\n";

        zmq::context_t contextSubExchange (1);
        zmq::socket_t ackSubscriber(contextSubExchange, ZMQ_SUB); // for recv ACKs from exchange.
        ackSubscriber.connect("tcp://127.0.0.1:5558"); // can be remapped using docker.
        ackSubscriber.setsockopt(ZMQ_RCVTIMEO, 1);

        while(true){
            // get newest market data
            // SYSTEM BLOCKING CALL
            zmq::message_t update;
            subscriber.recv (&update);
            std::string updt = std::string(static_cast<char*>(update.data()), update.size());
            std::cout << "Received Msg: " << updt << ":" <<update.size() << std::endl;
            if(updt[0] == 'Q'){
                bidAskQ.addingQuotesIntoQueues(updt);
            }
            else{
                std::stringstream parser(updt);
                int idx = 0;
                std::string temp;
                while(std::getline(parser, temp, ';')){
                    if(idx == 3){
                        cumulativeMarketVol += stof(temp);
                    }
                    idx++;
                }
            }
            // END

            // invoke algo
            std::vector<FIX::order> newOrders;
            tradeAlgo.execute(newOrders, bidAskQ, cumulativeMarketVol, position, orderID, senderCompID, openOrders); // execute the strategy
            FIX::sendAllMessages(newOrders, publisher, algoEngineLog);

            // del order if is ACK cancel
            while(true){
                ackSubscriber.recv(&update, ZMQ_NOBLOCK);
                updt = std::string(static_cast<char*>(update.data()), update.size());
                if(update.size() == 0){
                    break;
                }
                std::cout << "Received ACK Msg: " << updt << ":" <<update.size() << std::endl;
                FIX::ACK ackFromExchange(updt);
                if(ackFromExchange.MsgType == "4"){
                    // ACK FILLED
                    std::cout << "ORDER FILLED!!    " << ackFromExchange.to_string() << std::endl;
                    for(int i = 0; i < openOrders.size(); i++){
                        if(openOrders[i].OrderID == ackFromExchange.OrderID){
                            openOrders[i].OrderQty -= ackFromExchange.OrderQty;
                            tradeAlgo.targetQuantity -= ackFromExchange.OrderQty;
                            if(openOrders[i].OrderQty == 0){
                                openOrders.erase(openOrders.begin()+i);
                            }
                            break;
                        }
                    }
                }
                if(ackFromExchange.MsgType == "5"){
                    // ACK CANCELLED
                    std::cout << "ORDER CANCELLED!!    " << ackFromExchange.to_string() << std::endl;
                    for(int i = 0; i < openOrders.size(); i++){
                        if(openOrders[i].OrderID == ackFromExchange.OrderID){
                            openOrders.erase(openOrders.begin()+i);
                            break;
                        }
                    }
                    
                }
            }
        }
    }
    
}

int main(){
    ALGO::POVAlgo algo;
    TRADE::AlgoEngine<ALGO::POVAlgo> instance(algo);
    // std::invoke(TRADE::AlgoEngine<dummyAlgo>::run(), instance);
    instance.run();
}