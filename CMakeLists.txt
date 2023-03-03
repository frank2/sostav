cmake_minimum_required(VERSION 3.24)

project(sostav CXX)

option(SOSTAV_TEST "Enable testing for sostav." OFF)
#option(SOSTAV_BUILD_SHARED "Compile sostav as a shared library." OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE HEADER_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE SRC_FILES FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*.cpp)

source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE "${PROJECT_SOURCE_DIR}" PREFIX "Source Files" FILES ${SRC_FILES})

# disable document generation in libmikmod
option(ENABLE_DOC "" OFF)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib/mikmod/libmikmod)

#if (SOSTAV_BUILD_SHARED)
#  add_compile_definitions(SOSTAV_SHARED)
#  add_compile_definitions(SOSTAV_EXPORT)
#  add_library(sostav SHARED ${HEADER_FILES} ${PROJECT_SOURCE_DIR}/include/sostav.hpp ${SRC_FILES})
#else()
add_compile_definitions(SOSTAV_STATIC)
add_compile_definitions(MIKMOD_STATIC)
add_library(sostav STATIC ${HEADER_FILES} ${SRC_FILES})
#endif()

add_compile_definitions(UNICODE)
add_compile_definitions(_UNICODE)

target_link_libraries(sostav dsound)
target_link_libraries(sostav msimg32)
target_link_libraries(sostav mikmod)

include_directories(${PROJECT_SOURCE_DIR}/include)
set_target_properties(sostav PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(sostav PUBLIC
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/lib/mikmod/libmikmod/include"
)

if (SOSTAV_TEST)
  enable_testing()

  file(GLOB_RECURSE TEST_HEADERS FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/test/*.h ${PROJECT_SOURCE_DIR}/test/*.hpp)
  file(GLOB_RECURSE TEST_SOURCE FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/test/*.c ${PROJECT_SOURCE_DIR}/test/*.cpp)
  file(GLOB_RECURSE TEST_RSRC FOLLOW_SYMLINKS ${PROJECT_SOURCE_DIR}/test/*.rc)

  add_executable(testsostav WIN32 ${TEST_HEADERS} ${TEST_SOURCE})
  target_sources(testsostav PRIVATE ${TEST_RSRC})
  target_link_libraries(testsostav PUBLIC sostav)
  target_include_directories(testsostav PUBLIC
    "${PROJECT_SOURCE_DIR}/test"
  )
  add_test(NAME testsostav COMMAND testsostav)
endif()
