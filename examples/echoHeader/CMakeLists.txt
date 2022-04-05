add_executable(echoServerHeader
    echoServer.cxx
)
target_link_libraries(echoServerHeader 
  PRIVATE
    tev
)

add_executable(echoClientHeader
    echoClient.cxx
)
target_link_libraries(echoClientHeader 
  PRIVATE
    tev
)