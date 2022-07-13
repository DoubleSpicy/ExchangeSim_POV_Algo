// algo engine
#include "algoEngine.hpp"

namespace TRADE{
    template <class T> 
    AlgoEngine<T>::AlgoEngine(const T & algorithm): tradeAlgo(algorithm){};

    template <class T> 
    void AlgoEngine<T>::run(){
        while(true){
            
        }
    }
    
}