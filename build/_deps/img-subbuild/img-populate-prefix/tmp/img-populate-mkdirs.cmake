# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-src"
  "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-build"
  "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-subbuild/img-populate-prefix"
  "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-subbuild/img-populate-prefix/tmp"
  "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-subbuild/img-populate-prefix/src/img-populate-stamp"
  "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-subbuild/img-populate-prefix/src"
  "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-subbuild/img-populate-prefix/src/img-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-subbuild/img-populate-prefix/src/img-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/louiseperidy/Desktop/Tower-Defense/build/_deps/img-subbuild/img-populate-prefix/src/img-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
