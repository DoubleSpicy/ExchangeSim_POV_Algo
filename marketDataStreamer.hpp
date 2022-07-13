#include <string>
#include <zmq.hpp>
#include <signal.h>
#include <unistd.h>
// #include "zhelpers.hpp"

int main(){
    zmq::context_t context (1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcxp://*:5556");
    zmq::message_t msg(1);

    std::stringstream ss;
    // std::string counter = "0";
    while (1) {
        // Send current clock (msecs) to subscribers
        memcpy(msg.data(), "abc", 3);
        publisher.send(msg);
        msg.rebuild();
        usleep(1);
    }
    return 0;
}