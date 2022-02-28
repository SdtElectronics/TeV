add_library(streambuf OBJECT
    streambuf.cxx
)
target_include_directories(streambuf 
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/asio
)

add_library(tcp_response OBJECT
    tcpResponse.cxx
)
target_include_directories(tcp_response 
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/asio
)

add_library(tcp_request OBJECT
    tcpRequest.cxx
)
target_include_directories(tcp_request 
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/asio
)

if(MSVC)
    set_target_properties(
        streambuf
        tcp_response
        tcp_request

      PROPERTIES 
        COMPILE_OPTIONS /W4
    )
else()
    set_target_properties(
        streambuf
        tcp_response
        tcp_request

      PROPERTIES 
        COMPILE_OPTIONS -Wall -Wextra -Wpedantic
    )
endif()


add_library(tev
    tcpSession.cxx
)
target_link_libraries(tev 
  PRIVATE
    streambuf
    tcp_response
    tcp_request
  INTERFACE
    Threads::Threads
)

target_include_directories(tev 
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)