# Install script for directory: /home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenXLSX/headers" TYPE FILE FILES "/home/obraganca/Documentos/IC/optHirrygated/cmake-build-debug/lib/OpenXLSX/OpenXLSX/OpenXLSX-Exports.hpp")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenXLSX/headers" TYPE FILE FILES
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/IZipArchive.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLCell.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLCellIterator.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLCellRange.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLCellReference.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLCellValue.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLColor.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLColumn.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLCommandQuery.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLComments.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLConstants.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLContentTypes.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLDateTime.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLDocument.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLDrawing.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLException.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLFormula.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLIterator.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLMergeCells.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLProperties.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLRelationships.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLRow.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLRowData.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLSharedStrings.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLSheet.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLStyles.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLTables.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLWorkbook.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLXmlData.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLXmlFile.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLXmlParser.hpp"
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/headers/XLZipArchive.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenXLSX" TYPE FILE FILES "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/OpenXLSX.hpp")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/obraganca/Documentos/IC/optHirrygated/cmake-build-debug/output/libOpenXLSXd.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX" TYPE FILE FILES
    "/home/obraganca/Documentos/IC/optHirrygated/lib/OpenXLSX/OpenXLSX/OpenXLSXConfig.cmake"
    "/home/obraganca/Documentos/IC/optHirrygated/cmake-build-debug/lib/OpenXLSX/OpenXLSX/OpenXLSX/OpenXLSXConfigVersion.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX/OpenXLSXTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX/OpenXLSXTargets.cmake"
         "/home/obraganca/Documentos/IC/optHirrygated/cmake-build-debug/lib/OpenXLSX/OpenXLSX/CMakeFiles/Export/c72cc94553a1a0c9b05f75dae42fb1d7/OpenXLSXTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX/OpenXLSXTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX/OpenXLSXTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX" TYPE FILE FILES "/home/obraganca/Documentos/IC/optHirrygated/cmake-build-debug/lib/OpenXLSX/OpenXLSX/CMakeFiles/Export/c72cc94553a1a0c9b05f75dae42fb1d7/OpenXLSXTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/OpenXLSX" TYPE FILE FILES "/home/obraganca/Documentos/IC/optHirrygated/cmake-build-debug/lib/OpenXLSX/OpenXLSX/CMakeFiles/Export/c72cc94553a1a0c9b05f75dae42fb1d7/OpenXLSXTargets-debug.cmake")
  endif()
endif()

