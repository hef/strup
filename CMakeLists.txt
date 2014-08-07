cmake_minimum_required(VERSION 2.8)
project(strup)

list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS} -g")
add_subdirectory(slist)
add_subdirectory(markov)
add_subdirectory(irc)

