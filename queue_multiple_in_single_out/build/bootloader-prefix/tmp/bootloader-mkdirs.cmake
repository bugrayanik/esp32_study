# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/bugra/esp/esp-idf/components/bootloader/subproject"
  "/home/bugra/esp/deneme/queue_multiple_in_single_out/build/bootloader"
  "/home/bugra/esp/deneme/queue_multiple_in_single_out/build/bootloader-prefix"
  "/home/bugra/esp/deneme/queue_multiple_in_single_out/build/bootloader-prefix/tmp"
  "/home/bugra/esp/deneme/queue_multiple_in_single_out/build/bootloader-prefix/src/bootloader-stamp"
  "/home/bugra/esp/deneme/queue_multiple_in_single_out/build/bootloader-prefix/src"
  "/home/bugra/esp/deneme/queue_multiple_in_single_out/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/bugra/esp/deneme/queue_multiple_in_single_out/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/bugra/esp/deneme/queue_multiple_in_single_out/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()