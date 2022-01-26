#ifndef tevtcpsession
#define tevtcpsession

#include <memory>
#include <utility>

#include "asio/asio.hpp"
#include "internal/tevdefs.h"
#include "session.h"
#include "tcp/tcpSession.h"

namespace tev{
namespace internal{

using asio::ip::tcp;

class TCPresponse: public TCPsession{
  public:
    TEVINLINE
    TCPresponse(tcp::socket socket, std::shared_ptr<Session> session, std::size_t maxLength);

    TEVINLINE void start();
};

}
}

#ifdef TEVHDRONLY
#include "src/tcpResponse.cxx"
#endif

#endif