add_executable(echoServerNulTer
    echoServer.cxx
)
target_link_libraries(echoServerNulTer 
  PRIVATE
    tev
)

add_executable(echoClientNulTer
    echoClient.cxx
)
target_link_libraries(echoClientNulTer 
  PRIVATE
    tev
)