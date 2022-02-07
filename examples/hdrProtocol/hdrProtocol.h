/*
*
* structure of message
* +------------+--------------------+
* |    size    |       Payload      | 
* +------------+--------------------+
* |-  4-byte  -|-   length = size  -|
*
* header: contains a single 4-byte field size
* payload: arbitrary data with size bytes, binary safe
*
*/

#ifndef tevhdrprotocol
#define tevhdrprotocol

#include "sharedBuf.h"
#include "tcp/tcpSession.h"

using namespace tev;

class headerProtocol{
  public:
    template<typename CB>
    static void receive(TCPsession& session, CB callback);
    template<typename CB>
    static void send(TCPsession& session, const std::string& msg, CB callback);

  private:
    static inline void utob(uint32_t val, char* bytes);
    static inline uint32_t btou(const char* bytes);
};

template<typename CB>
void headerProtocol::receive(TCPsession& session, CB callback){
    session.asyncRead(4UL, [callback](TCPsession& session_){
        std::string msg = session_.getMessage();
        const std::size_t length = btou(msg.c_str());
        session_.asyncRead(length, 
        [callback](TCPsession& session__){
            callback(session__);
        });
    });
}

template<typename CB>
void headerProtocol::send(TCPsession& session, const std::string& msg, CB callback){
    char header[4];
    utob(msg.size(), header);
    session.asyncWrite(SharedBuffer(std::string(header, 4)).append(msg), 
    [callback](TCPsession& session_){
        callback(session_);
    });
}

void headerProtocol::utob(uint32_t val, char* bytes){
    bytes[0] = (val >> 24) & 0xFF;
    bytes[1] = (val >> 16) & 0xFF;
    bytes[2] = (val >> 8)  & 0xFF;
    bytes[3] =  val        & 0xFF;
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