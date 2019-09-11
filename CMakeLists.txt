# $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(CpFM)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
#----------------------------------------------------------------------------
# Find ROOT (optional package)
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /etc/root/cmake/)
#include(FindROOT.cmake)
find_package(ROOT)
if(ROOT_FOUND)
  add_definitions(-DG4ANALYSIS_USE_ROOT)
else()
  set(ROOTSYS /home/gred/root_34_36)
  set(ROOT_VERSION 5.34/36)
  set(ROOT_INCLUDE_DIR ${ROOTSYS}/include)
  set(ROOT_LIBRARY_DIR ${ROOTSYS}/lib)
  set(ROOT_LIBRARIES -LIBPATH:${ROOT_LIBRARY_DIR} -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic)
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
MESSAGE( STATUS "Geant4_INCLUDE_DIR:         " ${Geant4_INCLUDE_DIR} )
MESSAGE( STATUS "ROOT_INCLUDE_DIR:           " ${ROOT_INCLUDE_DIR} )

include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${ROOT_INCLUDE_DIR})
                    
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Exe main.cc ${sources} ${headers})

EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})

target_link_libraries(Exe ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(CPFM_SCRIPTS
  run.mac
  vis.mac
  #runAll.bash
  )

foreach(_script ${CPFM_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(xxx DEPENDS EXE)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Exe DESTINATION bin)
