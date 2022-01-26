
#ifndef tevhdrprotocol
#define tevhdrprotocol

#include "tcp/tcpSession.h"

using namespace tev;

class headerProtocol{
  public:
    template<typename CB>
    static void receive(TCPsession& session, CB callback);
    template<typename CB>
    static void send(TCPsession& session, std::string& msg, CB callback);

    static inline void utob(uint32_t val, char* bytes);
    static inline uint32_t btou(const char* bytes);
};

template<typename CB>
void headerProtocol::receive(TCPsession& session, CB callback){
    session.asyncRead(4UL, [callback](TCPsession& session_, const std::string& msg){
        const std::size_t length = btou(msg.c_str());
        session_.asyncRead(length, 
        [callback](TCPsession& session__, const std::string& msg){
            callback(session__, msg);
        });
    });
}

template<typename CB>
void headerProtocol::send(TCPsession& session, std::string& msg, CB callback){
    char header[4];
    utob(msg.size(), header);
    msg.insert(0, header, 4);
    session.asyncWrite(msg, [callback](TCPsession& session_){
        callback(session_);
    });
}

void headerProtocol::utob(uint32_t val, char* bytes){
    bytes[0] = (val >> 24)  & 0xFF;
    bytes[1] = (val >> 16)  & 0xFF;
    bytes[2] = (val >> 8)   & 0xFF;
    bytes[3] =  val         & 0xFF;
}

uint32_t headerProtocol::btou(const char* bytes){
    uint32_t ret = 0;
    ret |= bytes[3];
    ret |= (bytes[2] << 8);
    ret |= (bytes[1] << 16);
    ret |= (bytes[0] << 24);
    return ret;
}

#endif