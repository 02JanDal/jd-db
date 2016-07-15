cmake_minimum_required(VERSION 3.1)

project(jd-db)

find_package(Qt5 REQUIRED COMPONENTS Core Sql)

if(NOT TARGET jd-util)
	message(STATUS "Including jd-util from jd-db")
	add_subdirectory(../jd-util jd-util)
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)

include(Helpers)

set(SRC
	Database.h
	Database.cpp
	Column.h
	Column.cpp
	Table.h
	Table.cpp
	SelectBuilder.h
	SelectBuilder.cpp
)
prepend_to_list(SRC src/jd-db/)

add_library(jd-db ${SRC})
target_link_libraries(jd-db Qt5::Core Qt5::Sql jd-util)
target_include_directories(jd-db PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)

install(TARGETS jd-db EXPORT jd-db DESTINATION lib)
install(DIRECTORY src/jd-db DESTINATION include/jd-db FILES_MATCHING PATTERN "*.h")
install(EXPORT jd-db NAMESPACE jd_ DESTINATION lib/cmake)
install(DIRECTORY cmake/ DESTINATION lib/cmake/jd-db)

enable_testing()
add_subdirectory(test)
