# Copyright 2013 Corgan Labs
# This file is part of the Zerocoin project
# See LICENSE file or http://opensource.org/licenses/MIT for terms

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in-tree build. This is bad practice.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

#
# Setup overall project
#
cmake_minimum_required(VERSION 2.8)
project(zerocoin)

# Appended to CMAKE_INSTALL_PREFIX
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

#
# Project-wide package dependencies
#
# Find and configure the Boost C++ libraries
include(BoostConfig)

# Find and configure OpenSSL crypto library
include(FindOpenSSL)

#
# Project-wide directory settings
#
set(BITCOIN_BIGNUM_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/bitcoin_bignum)

#
# Project-wide installation settings
#
set(ZEROCOIN_INCLUDE_DIR include)
set(ZEROCOIN_BIN_DIR     bin)
set(ZEROCOIN_LIB_DIR     lib)

#
# Add individual directories to project
#
#add_subdirectory(foo)

#
# Create uninstall target
#
configure_file(
  ${CMAKE_SOURCE_DIR}/CMake/cmake_uninstall.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
@ONLY)

add_custom_target(uninstall
  ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
)

########################################################################
# Shared library generation libzerocoin.so
########################################################################

include_directories(
  ${BITCOIN_BIGNUM_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIR}
)

link_directories(
  ${Boost_LIBRARY_DIRS}
)

list(APPEND zerocoin_libs
  ${Boost_LIBRARIES}
  ${OPENSSL_CRYPTO_LIBRARY}
)

list(APPEND zerocoin_sources
  Accumulator.cpp
  AccumulatorProofOfKnowledge.cpp
  Coin.cpp
  CoinSpend.cpp
  Commitment.cpp
  ParamGeneration.cpp
  Params.cpp
  SerialNumberSignatureOfKnowledge.cpp
  SpendMetaData.cpp
)

add_library(zerocoin SHARED ${zerocoin_sources})
target_link_libraries(zerocoin ${zerocoin_libs})

########################################################################
# Executable files
########################################################################

add_executable(paramgen paramgen.cpp)
target_link_libraries(paramgen zerocoin)

add_executable(benchmark Benchmark.cpp)
target_link_libraries(benchmark zerocoin)

add_executable(test Tests.cpp)
target_link_libraries(test zerocoin)

add_executable(tutorial Tutorial.cpp)
target_link_libraries(tutorial zerocoin)
