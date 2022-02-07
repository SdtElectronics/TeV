# TeV
Thin wrapper of boost.asio for creating applications based on asynchronous TCP sockets.

* Based on asio library and therefore gained the same cross-platform ability
* C++ 11 compatible
* Deliberately avoid using `std::function<>` to reduce overhead of callbacks
* Presents extremely simple interfaces supporting two styles of session definition:
<table>
<tr> 
<td>
Session Defined with Class
</td>
<td>
Session Defined with Functions
</td>
</tr>
<tr>
<td>
<pre lang="C++">
class Server: public Session{
  public:
    virtual void onConnect     (TCPsession& session)override{
        doRead(session);
    }

    virtual void onDisconnect  (TCPsession& session)override{
        std::cout << session.getRemoteIP() 
           << ':' << session.getRemotePort() 
           << " disconnected" << std::endl;
    }

    virtual void onError(TCPsession& session, 
                         const std::string& err, 
                         ErrorType)override{
        std::cout << err << std::endl;
    }

    virtual ~TimeoutServer(){}

  private:
    void doRead(TCPsession& session){
        session.asyncRead('\0', 
        [](TCPsession& session_){
            std::cout << session_.getMessage() << std::endl;
            doRead(session_);
        });
    }
};

int main(){
    asio::io_context io_context;
    unsigned short port = 1701;
    Service<> srv(port, io_context);
    srv.start([]{return std::make_shared<Server>();});
    io_context.run();
}
</pre>
</td>
<td>
<pre lang="C++">
void doRead(TCPsession& session){
    session.asyncRead('\0', 
    [](TCPsession& session_){
        std::cout << session_.getMessage() << std::endl;
        doRead(session_);
    });
}

int main(){
    asio::io_context io_context;
    
    auto session = SessionBuilder<>().onConnect([](TCPsession& session){
        doRead(session);
    }).onDisconnect([](TCPsession& session){
        std::cout << session.getRemoteIP() 
           << ':' << session.getRemotePort() 
           << " disconnected" << std::endl;
    }).onError([](TCPsession& session, const std::string& err, ErrorType){
        std::cout << err << std::endl;
    }).buildFactory();

    unsigned short port = 1701;
    Service<> srv(port, io_context);
    srv.start(session);
    io_context.run();
}
</pre>
</td>
</tr>
</table>

## Examples
* Echo client and server, messages terminated with `'\0'`: [echoNulTer](examples/echoNulTer)
* Echo client and server, messages with header indicating the length: [echoHeader](examples/echoHeader)
* Echo client and server, session defined by chain of callbacks: [builder](examples/builder)
* Timeout server: [timeout](examples/timeout)

## References
### TcpSession
`TCPsession` encapsulated operations to be applied on the socket, including read, write, close, stat etc. This class should not be instantiated, but is passed to user-defined callbacks by reference.
#### Member functions:
* #### `void asyncRead(std::size_t length, CB&& callback)`

    Read `length` bytes of data from socket, and invoke `callback` upon finish. The signature of `CB` should be `void (TCPsession&)`

* #### `void asyncRead(char token, CB&& callback)`

    Read data from socket until `token` is met, then invoke `callback`. The signature of `CB` should be `void (TCPsession&)`

* #### `void asyncRead(const char* token, CB&& callback)`

    Read data from socket until `token` is met, then invoke `callback`. The signature of `CB` should be `void (TCPsession&)`

* #### `void asyncRead(CF&& completionCondition, CB&& callback)`

    Read data from socket until `completionCondition` returns 0, then invoke `callback`. The signature of `CB` should be `void (TCPsession&)`

* #### `void asyncWrite(const std::string& message, CB&& callback)`

    Write `message` to socket, and invoke `callback` upon finish. The signature of `CB` should be `void (TCPsession&)`. `message` won't be copied, so this reference must stay valid until `callback` is called.

* #### `void asyncWrite(SharedBuffer bufs, CB&& callback)`

    Write content in `bufs` to socket, and invoke `callback` upon finish. The signature of `CB` should be `void (TCPsession&)`. `SharedBuffer` will manage underlying memory, so no special care need to be paid on lifetime.

* #### `Timer defer(const duration<Rep, Period>& timeout, CB&& callback)`

    Invoke `callback` after `timeout`. The signature of `CB` should be `void (const std::error_code&)`.

* #### `Timer expire(const duration<Rep, Period>& timeout)`

    Close session after `timeout`

* #### `Timer expire(const duration<Rep, Period>& timeout, CB&& callback)`

    Invoke `callback` and close session after `timeout`

* #### `void close()`

    Close the session gracefully

* #### `std::string getMessage()`

    Get the received message after `asyncRead()`

* #### `std::string getMessage(std::size_t length)`

    Read `length` bytes of the received message after `asyncRead()`

* #### `std::string getRemoteIP()`

    Get the IP address of the remote host

* #### `unsigned short getRemotePort()`

    Get the port of the remote host

### TcpService
