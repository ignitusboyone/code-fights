cmake_minimum_required(VERSION 3.6.0)
cmake_policy(VERSION 3.6.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/)
option(CF_BUILD_PRETTY FALSE)

if( NOT CMAKE_BUILD_TYPE )
  set(BUILD_OPTIONS_STRINGS
    "Debug"
    "Release"
  )
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Release;Debug" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${BUILD_OPTIONS_STRINGS})
endif()
if( NOT CMAKE_DEBUG_POSTFIX )
  set(CMAKE_DEBUG_POSTFIX "_d" CACHE "Library Postfix for Debug Builds" STRING FORCE)
endif()

project(code_fights VERSION 0.0.5 LANGUAGES CXX Go Rust)

if(CF_BUILD_PRETTY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/usr/lib )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/usr/bin )
  set(CMAKE_PDB_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/usr/symbols )
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/usr/bin )
endif()

find_package(GTest)

set(CXX-Challanges 
  conwaysVariationsOfLife
  uShouldDate
  glasses
  square-diff
  transformLetters
)

set(GO-Challanges 
  uShouldDate
  glasses
  square-diff
  transformLetters
)

set(Rust-Challanges 
  uShouldDate
  glasses
  square-diff
  transformLetters
)
include_directories( ${GTEST_INCLUDE_DIR} )

foreach (proj IN LISTS CXX-Challanges)
    add_executable(${proj} source/cpp/${proj}.cpp)
    set_target_properties(${proj}
    PROPERTIES
    LANGUAGE CXX
    LINKER_LANGUAGE CXX
    DEBUG_POSTFIX _d
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/cpp"
    OUTPUT_NAME ${proj}
    CXX_STANDARD 14
    )
    target_link_libraries(${proj} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endforeach()

foreach (proj IN LISTS GO-Challanges)
    add_executable(go_${proj} source/go/${proj}.go)
    set_target_properties(go_${proj}
     PROPERTIES
     LANGUAGE Go
     LINKER_LANGUAGE Go
     DEBUG_POSTFIX _d
     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/go"
     OUTPUT_NAME ${proj}
     )
endforeach()

foreach (proj IN LISTS Rust-Challanges)
    add_executable(rust_${proj} source/rust/${proj}.rs)
    set_target_properties(rust_${proj}
     PROPERTIES
     LANGUAGE Rust
     LINKER_LANGUAGE Rust
     DEBUG_POSTFIX _d
     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/rust"
     OUTPUT_NAME ${proj}
     )
endforeach()

