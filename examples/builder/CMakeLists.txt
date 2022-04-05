add_executable(echoServerByBuilder
    echoServer.cxx
)
target_link_libraries(echoServerByBuilder 
  PRIVATE
    tev
)

add_executable(echoClientByBuilder
    echoClient.cxx
)
target_link_libraries(echoClientByBuilder 
  PRIVATE
    tev
)