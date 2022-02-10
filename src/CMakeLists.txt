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
    target_compile_options(tcp_response
      PUBLIC
        /W4
    )

    target_compile_options(tcp_request
      PUBLIC
        /W4
    )
else()
    target_compile_options(tcp_response
      PUBLIC
        -Wall -Wextra -Wpedantic
    )

    target_compile_options(tcp_request
      PUBLIC
        -Wall -Wextra -Wpedantic
    )
endif()


add_library(tev
    tcpSession.cxx
)
target_link_libraries(tev 
  PRIVATE
    tcp_response
    tcp_request
  INTERFACE
    Threads::Threads
)

target_include_directories(tev 
  INTERFACE
    ${PROJECT_SOURCE_DIR}/include
)