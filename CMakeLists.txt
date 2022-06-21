cmake_minimum_required(VERSION 3.15)
project(code)

set(CXX_FLAGS -std=c++17 -Ofast)

# add_executable(code data.hpp string.cpp user.hpp HashMap.h cache.h Bptest.cpp Bpt_and_database.cpp Bpt_and_database.h)
# add_executable(code main.cpp data.hpp string.cpp user.hpp HashMap.h B+Tree.hpp log.hpp)
add_executable(code main.cpp data.hpp string.cpp user.hpp HashMap.h BPlusTree.h log.hpp)