# /**
#  * @file:		CMakeLists.txt
#  * @author:		胡文博
#  * @email:		huwenbo@mail.dlut.edu.cn
#  * @dateTime:		2017-04-30 12:31:03
#  * @description: 	
#  */
project(ncc)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("./src/virtualMachine")
aux_source_directory(./src/virtualMachine VMSRC_LIST)
add_executable( "nvm" ${VMSRC_LIST} )

include_directories("./src")
aux_source_directory(./src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
install (TARGETS nvm DESTINATION bin)
