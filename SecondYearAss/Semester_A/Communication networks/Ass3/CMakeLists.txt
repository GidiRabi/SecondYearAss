cmake_minimum_required(VERSION 3.27)
project(network_ex3_master C)

set(CMAKE_C_STANDARD 11)

include_directories("part B")

add_executable(network_ex3_master
        examples/tcp/tcp_client.c
        examples/tcp/tcp_server.c
        examples/upd/udp_client.c
        examples/upd/udp_server.c
        "part A/TCP_Reciever.c"
        "part A/TCP_Sender.c"
        "part B/RUDP_API.c"
        "part B/RUDP_API.h"
        "part B/RUDP_Reciever.c"
        "part B/RUDP_Sender.c")
