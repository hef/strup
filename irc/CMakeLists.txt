add_library(irc client.cpp)
add_executable(ircbot main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/markov ${CMAKE_SOURCE_DIR}/slist)
target_link_libraries(ircbot irc markov slist)
