#include <iostream>
#include <zmq.hpp>

int main (int argc, char *argv[])
{
zmq::context_t context (1);
zmq::socket_t subscriber (context, ZMQ_SUB);
subscriber.connect("tcp://127.0.0.1:5556");
subscriber.setsockopt(ZMQ_SUBSCRIBE, "T", 1); // subscribe to all messages
subscriber.setsockopt(ZMQ_SUBSCRIBE, "Q", 1);
//  Process 10 updates
// int update_nbr;
while(true) {
    zmq::message_t update;
    subscriber.recv (&update);
    std::string updt = std::string(static_cast<char*>(update.data()), update.size());
    std::cout << "Received Update/Messages/TaskList " <<" : "<< updt << ":" <<update.size() << std::endl;
}
return 0;
}