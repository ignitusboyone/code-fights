
#=============================================================================
# Copyright 2004-2011 Kitware, Inc.
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

# This should be included before the _INIT variables are
# used to initialize the cache.  Since the rule variables
# have if blocks on them, users can still define them here.
# But, it should still be after the platform file so changes can
# be made to those values.

if(CMAKE_USER_MAKE_RULES_OVERRIDE)
  # Save the full path of the file so try_compile can use it.
  include(${CMAKE_USER_MAKE_RULES_OVERRIDE} RESULT_VARIABLE _override)
  set(CMAKE_USER_MAKE_RULES_OVERRIDE "${_override}")
endif()

if(CMAKE_USER_MAKE_RULES_OVERRIDE_Rust)
  # Save the full path of the file so try_compile can use it.
   include(${CMAKE_USER_MAKE_RULES_OVERRIDE_Rust} RESULT_VARIABLE _override)
   set(CMAKE_USER_MAKE_RULES_OVERRIDE_Rust "${_override}")
endif()

if(NOT CMAKE_Rust_COMPILE_OBJECT)
  set(CMAKE_Rust_COMPILE_OBJECT "rustc -o <TARGET> <SOURCE> ")
endif()

if(NOT CMAKE_Rust_LINK_LIBRARY)
  set(CMAKE_Rust_LINK_EXECUTABLE "")
endif()

if(NOT CMAKE_Rust_LINK_EXECUTABLE)
  set(CMAKE_Rust_LINK_EXECUTABLE "")
endif()
