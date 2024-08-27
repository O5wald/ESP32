# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/oswald/esp/esp-idf/components/bootloader/subproject"
  "/home/oswald/new_esp/esp-idf-template/build/bootloader"
  "/home/oswald/new_esp/esp-idf-template/build/bootloader-prefix"
  "/home/oswald/new_esp/esp-idf-template/build/bootloader-prefix/tmp"
  "/home/oswald/new_esp/esp-idf-template/build/bootloader-prefix/src/bootloader-stamp"
  "/home/oswald/new_esp/esp-idf-template/build/bootloader-prefix/src"
  "/home/oswald/new_esp/esp-idf-template/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/oswald/new_esp/esp-idf-template/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/oswald/new_esp/esp-idf-template/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
