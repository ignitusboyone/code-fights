cmake_minimum_required(VERSION 3.6.0)
cmake_policy(VERSION 3.6.0)

project(code_fights VERSION 0.0.1 LANGUAGES CXX Go)
option(CF_BUILD_PRETTY FALSE)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_DIR}/cmake)

if(CF_BUILD_PRETTY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/usr/lib )
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/usr/bin )
  set(CMAKE_PDB_OUTPUT_DIRECTORY     ${CMAKE_BINARY_DIR}/usr/symbols )
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/usr/bin )
endif()

find_package(GTest)

set(CXX-Challanges uShouldDate
  glasses
  square-diff
  transformLetters
  uShouldDate
)

set(GO-Challanges uShouldDate
  glasses
  square-diff
  transformLetters
  uShouldDate
)

include_directories( ${GTEST_INCLUDE_DIR} )

foreach (proj IN LISTS CXX-Challanges)
    add_executable(${proj} source/cpp/${proj}.cpp)
    set_target_properties(${proj}
    PROPERTIES
    LANGUAGE CXX
    LINKER_LANGUAGE CXX
    DEBUG_POSTFIX _d
    CXX_STANDARD 14
    
    )
    target_link_libraries(${proj} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endforeach()

foreach (proj IN LISTS GO-Challanges)
    add_executable(go_${proj} source/go/${proj}.go)
    set_target_properties(${proj}
    PROPERTIES
    LANGUAGE Go
    LINKER_LANGUAGE Go
    DEBUG_POSTFIX _d
    )
endforeach()

