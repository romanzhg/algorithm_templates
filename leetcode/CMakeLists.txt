# To enable asan.
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

file(GLOB files "lc727.cpp")
foreach(file ${files})
    #    message(${file})
    string(REPLACE "/home/romanzhg/ws/repos/oj/leetcode/" "" name ${file})
    string(REPLACE ".cpp" "" pure_name ${name})
    #    message(${pure_name})
    add_executable(${pure_name} ${file})
endforeach()
