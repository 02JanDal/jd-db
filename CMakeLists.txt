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

set(SRC_SCHEMA
	Column.h
	Column.cpp
	Table.h
	Table.cpp
)
prepend_to_list(SRC_SCHEMA src/jd-db/schema/)
add_library(jd-db-schema ${SRC_SCHEMA})
target_link_libraries(jd-db-schema Qt5::Core jd-util)
target_include_directories(jd-db-schema PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)

set(SRC_SQL
	Database.h
	Database.cpp
	Dialects.h
	Dialects.cpp
	SelectBuilder.h
	SelectBuilder.cpp
	CreateBuilder.h
	CreateBuilder.cpp
)
prepend_to_list(SRC_SQL src/jd-db/sql/)
add_library(jd-db-sql ${SRC_SQL})
target_link_libraries(jd-db-sql Qt5::Core Qt5::Sql jd-util jd-db-schema)
target_include_directories(jd-db-sql PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
	$<INSTALL_INTERFACE:include>
)

install(TARGETS jd-db-sql jd-db-schema EXPORT jd-db DESTINATION lib)
install(DIRECTORY src/jd-db DESTINATION include/jd-db FILES_MATCHING PATTERN "*.h")
install(EXPORT jd-db NAMESPACE jd_ DESTINATION lib/cmake)
install(DIRECTORY cmake/ DESTINATION lib/cmake/jd-db)

enable_testing()
add_subdirectory(test)
