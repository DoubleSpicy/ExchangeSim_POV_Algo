// algo engine
#include "algoEngine.hpp"

namespace TRADE{
    template <class T> 
    AlgoEngine<T>::AlgoEngine(const T & algorithm): tradeAlgo(algorithm){};

    template <class T> 
    void AlgoEngine<T>::run(){
        zmq::context_t context (1);
        zmq::socket_t publisher(context, ZMQ_PUB);
        publisher.bind("tcp://127.0.0.1:5556"); // can be remapped using docker.
        usleep(200000); 
        zmq::message_t msg(1);
        std::stringstream ss;
        while(true){
            
        }
    }
    
}