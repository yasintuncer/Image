# Install script for directory: /home/master/Yasin/Projects/Image/modules/psd_sdk

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/master/Yasin/Projects/Image/install")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/master/Yasin/Projects/Image/build/modules/psd_sdk/libpsd.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/master/Yasin/Projects/Image/install/include/Psd.h;/home/master/Yasin/Projects/Image/install/include/PsdAllocator.h;/home/master/Yasin/Projects/Image/install/include/PsdAlphaChannel.h;/home/master/Yasin/Projects/Image/install/include/PsdAssert.h;/home/master/Yasin/Projects/Image/install/include/PsdBitUtil.h;/home/master/Yasin/Projects/Image/install/include/PsdBlendMode.h;/home/master/Yasin/Projects/Image/install/include/PsdChannel.h;/home/master/Yasin/Projects/Image/install/include/PsdChannelType.h;/home/master/Yasin/Projects/Image/install/include/PsdColorMode.h;/home/master/Yasin/Projects/Image/install/include/PsdColorModeDataSection.h;/home/master/Yasin/Projects/Image/install/include/PsdCompilerMacros.h;/home/master/Yasin/Projects/Image/install/include/PsdCompressionType.h;/home/master/Yasin/Projects/Image/install/include/PsdDebug.h;/home/master/Yasin/Projects/Image/install/include/PsdDecompressRle.h;/home/master/Yasin/Projects/Image/install/include/PsdDocument.h;/home/master/Yasin/Projects/Image/install/include/PsdDocumentation.h;/home/master/Yasin/Projects/Image/install/include/PsdEndianConversion.h;/home/master/Yasin/Projects/Image/install/include/PsdExport.h;/home/master/Yasin/Projects/Image/install/include/PsdExportChannel.h;/home/master/Yasin/Projects/Image/install/include/PsdExportColorMode.h;/home/master/Yasin/Projects/Image/install/include/PsdExportDocument.h;/home/master/Yasin/Projects/Image/install/include/PsdExportLayer.h;/home/master/Yasin/Projects/Image/install/include/PsdExportMetaDataAttribute.h;/home/master/Yasin/Projects/Image/install/include/PsdFile.h;/home/master/Yasin/Projects/Image/install/include/PsdFixedSizeString.h;/home/master/Yasin/Projects/Image/install/include/PsdImageDataSection.h;/home/master/Yasin/Projects/Image/install/include/PsdImageResourceType.h;/home/master/Yasin/Projects/Image/install/include/PsdImageResourcesSection.h;/home/master/Yasin/Projects/Image/install/include/PsdInterleave.h;/home/master/Yasin/Projects/Image/install/include/PsdKey.h;/home/master/Yasin/Projects/Image/install/include/PsdLayer.h;/home/master/Yasin/Projects/Image/install/include/PsdLayerCanvasCopy.h;/home/master/Yasin/Projects/Image/install/include/PsdLayerMask.h;/home/master/Yasin/Projects/Image/install/include/PsdLayerMaskSection.h;/home/master/Yasin/Projects/Image/install/include/PsdLayerType.h;/home/master/Yasin/Projects/Image/install/include/PsdLog.h;/home/master/Yasin/Projects/Image/install/include/PsdMallocAllocator.h;/home/master/Yasin/Projects/Image/install/include/PsdMemoryUtil.h;/home/master/Yasin/Projects/Image/install/include/PsdNamespace.h;/home/master/Yasin/Projects/Image/install/include/PsdNativeFile.h;/home/master/Yasin/Projects/Image/install/include/PsdNativeFile_Linux.h;/home/master/Yasin/Projects/Image/install/include/PsdNativeFile_Mac.h;/home/master/Yasin/Projects/Image/install/include/PsdParseColorModeDataSection.h;/home/master/Yasin/Projects/Image/install/include/PsdParseDocument.h;/home/master/Yasin/Projects/Image/install/include/PsdParseImageDataSection.h;/home/master/Yasin/Projects/Image/install/include/PsdParseImageResourcesSection.h;/home/master/Yasin/Projects/Image/install/include/PsdParseLayerMaskSection.h;/home/master/Yasin/Projects/Image/install/include/PsdPch.h;/home/master/Yasin/Projects/Image/install/include/PsdPlanarImage.h;/home/master/Yasin/Projects/Image/install/include/PsdPlatform.h;/home/master/Yasin/Projects/Image/install/include/PsdSection.h;/home/master/Yasin/Projects/Image/install/include/PsdStringUtil.h;/home/master/Yasin/Projects/Image/install/include/PsdSyncFileReader.h;/home/master/Yasin/Projects/Image/install/include/PsdSyncFileUtil.h;/home/master/Yasin/Projects/Image/install/include/PsdSyncFileWriter.h;/home/master/Yasin/Projects/Image/install/include/PsdTgaExporter.h;/home/master/Yasin/Projects/Image/install/include/PsdThumbnail.h;/home/master/Yasin/Projects/Image/install/include/PsdTypes.h;/home/master/Yasin/Projects/Image/install/include/PsdUnionCast.h;/home/master/Yasin/Projects/Image/install/include/PsdVectorMask.h;/home/master/Yasin/Projects/Image/install/include/Psdinttypes.h;/home/master/Yasin/Projects/Image/install/include/Psdispod.h;/home/master/Yasin/Projects/Image/install/include/Psdisunsigned.h;/home/master/Yasin/Projects/Image/install/include/Psdminiz.h;/home/master/Yasin/Projects/Image/install/include/Psdstdint.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/master/Yasin/Projects/Image/install/include" TYPE FILE FILES
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/Psd.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdAllocator.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdAlphaChannel.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdAssert.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdBitUtil.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdBlendMode.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdChannel.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdChannelType.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdColorMode.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdColorModeDataSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdCompilerMacros.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdCompressionType.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdDebug.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdDecompressRle.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdDocument.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdDocumentation.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdEndianConversion.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdExport.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdExportChannel.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdExportColorMode.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdExportDocument.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdExportLayer.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdExportMetaDataAttribute.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdFile.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdFixedSizeString.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdImageDataSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdImageResourceType.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdImageResourcesSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdInterleave.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdKey.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdLayer.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdLayerCanvasCopy.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdLayerMask.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdLayerMaskSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdLayerType.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdLog.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdMallocAllocator.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdMemoryUtil.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdNamespace.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdNativeFile.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdNativeFile_Linux.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdNativeFile_Mac.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdParseColorModeDataSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdParseDocument.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdParseImageDataSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdParseImageResourcesSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdParseLayerMaskSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdPch.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdPlanarImage.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdPlatform.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdSection.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdStringUtil.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdSyncFileReader.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdSyncFileUtil.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdSyncFileWriter.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdTgaExporter.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdThumbnail.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdTypes.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdUnionCast.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdVectorMask.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/Psdinttypes.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/Psdispod.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/Psdisunsigned.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/Psdminiz.h"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/Psdstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/master/Yasin/Projects/Image/install/include/Psdminiz.c")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/master/Yasin/Projects/Image/install/include" TYPE FILE FILES "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/Psdminiz.c")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/master/Yasin/Projects/Image/install/include/PsdBitUtil.inl;/home/master/Yasin/Projects/Image/install/include/PsdEndianConversion.inl;/home/master/Yasin/Projects/Image/install/include/PsdMemoryUtil.inl;/home/master/Yasin/Projects/Image/install/include/PsdSyncFileUtil.inl;/home/master/Yasin/Projects/Image/install/include/PsdUnionCast.inl")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/master/Yasin/Projects/Image/install/include" TYPE FILE FILES
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdBitUtil.inl"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdEndianConversion.inl"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdMemoryUtil.inl"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdSyncFileUtil.inl"
    "/home/master/Yasin/Projects/Image/modules/psd_sdk/src/PsdUnionCast.inl"
    )
endif()

