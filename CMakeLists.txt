cmake_minimum_required(VERSION 3.28)
project(cw_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(extra/Brute_Force_algorithm)
include_directories(build)
include_directories(extra/Karp-Rabin_algorithm)
include_directories(extra/utils)

add_executable(cw_2
        extra/Brute_Force_algorithm/naive_search.h
        extra/Karp-Rabin_algorithm/rabin-karp.h
        extra/utils/utils.h
        main.cpp
        extra/Brute_Force_algorithm/naive_search.cpp
        extra/Karp-Rabin_algorithm/rabin_karp.cpp
        extra/utils/utils.cpp
        extra/kmp_search/kmp_search.cpp
        extra/kmp_search/kmp_search.h
        extra/bm_search/boyer_moore_search.cpp
        extra/bm_search/boyer_moore_search.h
        extra/z_search/z_search.cpp
        extra/z_search/z_search.h
        extra/aho_corasick/aho_corasick.h
        extra/aho_corasick/aho_corasick.cpp)
