# /**
#  * @file:		CMakeLists.txt
#  * @author:		胡文博
#  * @email:		huwenbo@mail.dlut.edu.cn
#  * @dateTime:		2017-04-30 12:31:03
#  * @description: 	
#  */
project(ncc)
cmake_minimum_required(VERSION 2.8)
# add_compile_options(-std=c++11)
# set(CMAKE_C_FLAGS "-m32")
# set(CMAKE_CXX_FLAGS "-m32")
include_directories("./src")
aux_source_directory(./src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
