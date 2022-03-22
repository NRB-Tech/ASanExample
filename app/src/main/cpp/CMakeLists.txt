cmake_minimum_required(VERSION 3.10)

project(ASanLibrary)

add_library(asanexample SHARED test.c)

if(SANITIZE)
    target_compile_options(asanexample PUBLIC -fsanitize=address -fno-omit-frame-pointer)
    set_target_properties(asanexample PROPERTIES LINK_FLAGS -fsanitize=address)
endif()
