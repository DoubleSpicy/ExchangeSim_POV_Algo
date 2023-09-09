// exchange mockup
#include "exchange.hpp"
#include <mutex>
#include <unistd.h>
// TODO: split m
namespace EXCHANGE{
    void TradeMatchingEngine::run(){
        std::ofstream exchangeLog("log.txt");

        zmq::context_t context(1);
        zmq::socket_t subscriber (context, ZMQ_SUB);
        subscriber.connect("tcp://127.0.0.1:5556");
        subscriber.setsockopt(ZMQ_SUBSCRIBE, "Q", 1); // subscribe to Q-typed market data
        std::cout << "running...market-sub\n";


        zmq::context_t clientContext(1);
        zmq::socket_t orderSubscriber (clientContext, ZMQ_SUB);
        orderSubscriber.bind("tcp://127.0.0.1:5557");
        orderSubscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0); // subscribe to all client trade orders
        std::cout << "running...order-sub\n";
        orderSubscriber.setsockopt(ZMQ_RCVTIMEO, 1);

        zmq::context_t publisherTowardsClients(1);
        zmq::socket_t ackPublisher (publisherTowardsClients, ZMQ_PUB);
        ackPublisher.connect("tcp://127.0.0.1:5558");
        usleep(200000); 
        std::cout << "running ack-publisher\n";
        zmq::message_t msg(1);

        while(true){
            zmq::message_t update;
            // keep recv order message, and try matching order, 
            subscriber.recv(&update); // get market data
            std::string updt = std::string(static_cast<char*>(update.data()), update.size());
            std::cout << "Received Market Msg: " << updt << ":" <<update.size() << std::endl;
            BidAsk.addingQuotesIntoQueues(updt); // put quotes into bidask

            // keep recv order/cancel message, and try matching order, 
            // send back ACK if is POV msg
            std::cout <<"waiting...\n";
            while(true){
                orderSubscriber.recv(&update, ZMQ_NOBLOCK);
                updt = std::string(static_cast<char*>(update.data()), update.size());
                if(update.size() == 0){
                    break;
                }
                std::cout << "Received Client Msg: " << updt << ":" <<update.size() << std::endl;
                FIX::order orderFromClient(updt);
                if(orderFromClient.MsgType == "0"){ // order
                    std::cout << "is order\n";
                    BidAsk.clientOrders.push_back(orderFromClient);
                    std::cout << BidAsk.clientOrders.size() << " queued." << std::endl;
                    // std::cout << currentClientOrders.back().to_string() << std::endl;
                    // TODO: send ack order msg
                    FIX::ACK ackOrderMsg(orderFromClient.SenderCompID, "3", orderFromClient.OrderID, -1, orderFromClient.Price);
                    std::string data = ackOrderMsg.to_string();
                    msg.rebuild(data.size());
                    ackPublisher.send(msg);
                }
                else{
                    std::cout << "is not order\n";
                    std::vector<FIX::ACK> cancelledOrders;
                    BidAsk.cancelOrder(orderFromClient, cancelledOrders);
                    FIX::sendAllMessages(cancelledOrders, ackPublisher, exchangeLog, cumulativeQty);
                }
            }

            std::vector<FIX::ACK> filledOrders;
            BidAsk.tryFill3MinsOrder(filledOrders);
            if(BidAsk.fillOrders(filledOrders)){
                std::cout <<"Order filled!\n";
            }
            else{
                std::cout <<"No order filled!\n";
            }
            FIX::sendAllMessages(filledOrders, ackPublisher, exchangeLog, cumulativeQty);
        }
    }

}



int main(){
    EXCHANGE::TradeMatchingEngine exchange;
    exchange.run();
}