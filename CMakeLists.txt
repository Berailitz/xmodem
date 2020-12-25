cmake_minimum_required(VERSION 3.17)
project(xmodem C)

set(CMAKE_C_STANDARD 90)

add_executable(xmodem statusdef.h log.c lock.h util.h main.c timer_test.h itimer.h uinterrupt.h led.h simenv.h concurrent.c itimer.c lock.c uinterrupt.c util.c led.c timer_test.c allocator.c pool.c linked.c mempool_test.c test.c)
