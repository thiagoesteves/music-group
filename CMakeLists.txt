cmake_minimum_required(VERSION 2.8.8)

# Options 'cmake -Dtest=ON'.
option(test "Build test target" OFF) # Makes 'test' available

set(PROJECT_NAME_STR music-group-stack)
project(${PROJECT_NAME_STR} C CXX)

find_package(Threads REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-Wall -ansi -Wno-deprecated -pthread)
endif()

if(MSVC)
    #vc 2012 fix for vararg templates
    set(MSVC_COMPILER_DEFS "-D_VARIADIC_MAX=10")
endif() 
#-------------------
# set common include folder for module
#-------------------
set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)

#-------------------
# Common Targets
#-------------------
if (NOT test)

  include_directories(${COMMON_INCLUDES})
  add_executable(${PROJECT_NAME_STR} ${PROJECT_SOURCE_DIR}/src/main.cpp)
  target_link_libraries(${PROJECT_NAME_STR} ${CMAKE_THREAD_LIBS_INIT})

#-------------------
# Test target
#-------------------
else()

  set(EXT_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
  add_subdirectory(${EXT_PROJECTS_DIR}/gtest)

  add_definitions(${MSVC_COMPILER_DEFS})
  enable_testing()
  set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}-gtest)
  include_directories(${GTEST_INCLUDE_DIRS} ${COMMON_INCLUDES})
  
  file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)
  add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
  add_dependencies(${PROJECT_TEST_NAME} googletest)
  
  
  if(NOT WIN32 OR MINGW)
      target_link_libraries(${PROJECT_TEST_NAME}
          ${GTEST_LIBS_DIR}/libgtest.a
          ${GTEST_LIBS_DIR}/libgtest_main.a
          )
  else()
      target_link_libraries(${PROJECT_TEST_NAME}
          debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
          optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
          )
      target_link_libraries(${PROJECT_TEST_NAME}
          debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
          optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
          )
  endif()
  
  target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})
  
  add_test(test1 ${PROJECT_TEST_NAME})
  
endif()
