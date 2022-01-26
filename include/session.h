
#ifndef tevsession
#define tevsession

#include <memory>
#include <string>

namespace tev{

class TCPsession;

class Session: public std::enable_shared_from_this<Session>{
  public:
    virtual void   onConnect     (TCPsession& session                        ) = 0;
    virtual void   onDisconnect  (TCPsession& session                        ) = 0;
    virtual void   onConnectError(TCPsession& session, const std::string& err) = 0;
    virtual void   onReadError   (TCPsession& session, const std::string& err) = 0;
    virtual void   onWriteError  (TCPsession& session, const std::string& err) = 0;
    virtual inline ~Session()                                                  = 0;
};

Session::~Session(){}

}
#endif