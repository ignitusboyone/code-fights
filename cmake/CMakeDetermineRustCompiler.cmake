#=============================================================================
# Copyright 2002-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# determine the compiler to use for Rust programs
# NOTE, a generator may set CMAKE_Rust_COMPILER before
# loading this file to force a compiler.

if(NOT CMAKE_Rust_COMPILER)
  # prefer the environment variable CC
  if(NOT $ENV{RUST_COMPILER} STREQUAL "")
    get_filename_component(CMAKE_Rust_COMPILER_INIT $ENV{RUST_COMPILER} PROGRAM PROGRAM_ARGS CMAKE_Rust_FLAGS_ENV_INIT)
    if(CMAKE_Rust_FLAGS_ENV_INIT)
      set(CMAKE_Rust_COMPILER_ARG1 "${CMAKE_Rust_FLAGS_ENV_INIT}" CACHE STRING "First argument to Rust compiler")
    endif()
    if(NOT EXISTS ${CMAKE_Rust_COMPILER_INIT})
      message(SEND_ERROR "Could not find compiler set in environment variable RUST_COMPILER:\n$ENV{RUST_COMPILER}.")
    endif()
  endif()

  set(Rust_BIN_PATH
    $ENV{RUSTPATH}
    $ENV{RUSTROOT}
    $ENV{RUSTROOT}/../bin
    $ENV{RUST_COMPILER}
    /usr/bin
    /usr/local/bin
    )
  # if no compiler has been specified yet, then look for one
  if(CMAKE_Rust_COMPILER_INIT)
    set(CMAKE_Rust_COMPILER ${CMAKE_Rust_COMPILER_INIT} CACHE PATH "Rust Compiler")
  else()
    find_program(CMAKE_Rust_COMPILER
      NAMES rustc
      PATHS ${Rust_BIN_PATH}
    )
  endif()
endif()
mark_as_advanced(CMAKE_Rust_COMPILER)

# configure variables set in this file for fast reload later on
configure_file(cmake/CMakeRustCompiler.cmake.in
  ${CMAKE_PLATFORM_INFO_DIR}/CMakeRustCompiler.cmake @ONLY)
set(CMAKE_Rust_COMPILER_ENV_VAR "RUST_COMPILER")
