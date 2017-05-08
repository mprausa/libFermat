cmake_minimum_required(VERSION 3.1)
project(libFermat)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR lib/cmake/libFermat CACHE PATH "Installation directory for CMake files")

foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/pstreams)

file(GLOB SOURCES src/*.cpp)
file(GLOB TOOL_SOURCES tool/*.cpp)

add_library(Fermat SHARED ${SOURCES})
add_executable(fermat ${TOOL_SOURCES})
target_link_libraries(fermat Fermat)

install (DIRECTORY include/ pstreams/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install (TARGETS Fermat EXPORT libFermatTargets DESTINATION lib)
install (TARGETS fermat DESTINATION bin)

file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_INCLUDE_DIR}")
file(RELATIVE_PATH REL_LIBRARY_DIR "${INSTALL_CMAKE_DIR}" "${INSTALL_LIB_DIR}")
set(CONF_INCLUDE_DIR "\${LIBFERMAT_CMAKE_DIR}/${REL_INCLUDE_DIR}")
set(CONF_LIBRARY_DIR "\${LIBFERMAT_CMAKE_DIR}/${REL_LIBRARY_DIR}")
configure_file(libFermatConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/libFermatConfig.cmake" @ONLY)

install(FILES "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/libFermatConfig.cmake" DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
install(EXPORT libFermatTargets DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

