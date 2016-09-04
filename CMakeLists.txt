cmake_minimum_required(VERSION 3.6.0)
cmake_policy(VERSION 3.6.0)

project(code_fights VERSION 0.0.1 LANGUAGES CXX)
option(CF_BUILD_PRETTY FALSE)

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
)

include_directories( ${GTEST_INCLUDE_DIR} )

foreach (proj IN LISTS CXX-Challanges)
    add_executable(${proj} source/${proj}.cpp)
    set_target_properties(${proj}
    PROPERTIES
    LANGUAGE CXX
    LINKER_LANGUAGE CXX
    DEBUG_POSTFIX _d
    CXX_STANDARD 14
    
    )
    target_link_libraries(${proj} ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})
endforeach()

