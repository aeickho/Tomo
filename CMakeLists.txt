# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(Tomo)

# force output into several folders
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/bin )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/bin )
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${PROJECT_BINARY_DIR}/bin )
SET (CMAKE_CURRENT_BINARY_DIR
  ${PROJECT_BINARY_DIR}/moc )
SET (CMAKE_PROJECT_DOC_DIR
  ${PROJECT_BINARY_DIR}/doc )

# requirements
FIND_PACKAGE(Qt4 REQUIRED)

# include directories
INCLUDE_DIRECTORIES(. ${CMAKE_CURRENT_BINARY_DIR} ./include ./TbdLib/include /usr/include/ImageMagick  )

# tomo library
AUX_SOURCE_DIRECTORY(./src/lib tomo_src)
AUX_SOURCE_DIRECTORY(./src/OpenMesh OpenMesh_src)
ADD_LIBRARY(tomo STATIC ${tomo_src})

# OpenMeshCore library
ADD_LIBRARY(OpenMeshCore SHARED ${OpenMesh_src})
SET_TARGET_PROPERTIES( OpenMeshCore PROPERTIES COMPILE_FLAGS " -pipe -O3 -DINCLUDE_TEMPLATES -W -Wall -Wno-unused -DNDEBUG -D_REENTRANT -fPIC " )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Qunused-arguments -ferror-limit=1")

# link with...
TARGET_LINK_LIBRARIES(tomo glut GLU GL OpenMeshCore )

# qt4
INCLUDE(${QT_USE_FILE})
SET(cg2ex1_SOURCES ./src/cg2ex1/glwidget.cpp ./src/cg2ex1/main.cpp ./src/cg2ex1/MainWindow.cpp)
SET(cg2ex1_HEADERS ./src/cg2ex1/glwidget.h ./src/cg2ex1/MainWindow.h)
SET(cg2ex1_FORMS   ./src/cg2ex1/MainWindow.ui)
QT4_WRAP_CPP(cg2ex1_HEADERS_MOC ${cg2ex1_HEADERS})
QT4_WRAP_UI(cg2ex1_FORMS_HEADERS ${cg2ex1_FORMS})
ADD_EXECUTABLE(cg2ex1 
  ${cg2ex1_SOURCES} 
  ${cg2ex1_HEADERS_MOC}
  ${cg2ex1_FORMS_HEADERS}
  )
TARGET_LINK_LIBRARIES(cg2ex1 tomo -lQtOpenGL -lQtGui -lQtCore -lGLU -lGL -lpthread -lrt)
ADD_DEFINITIONS(${QT_DEFINITIONS})

# Apps
# DebugSlicer
ADD_EXECUTABLE(tomo-debugslicer ./src/app/debugslicer/DebugSlicer.hpp ./src/app/debugslicer/debugslicer.cpp)
TARGET_LINK_LIBRARIES(tomo-debugslicer boost_program_options tomo Magick++ rt)

# Tests and Benchmarking
ADD_EXECUTABLE(TomoUnitTests ./src/test/UnitTests.cpp)
TARGET_LINK_LIBRARIES(TomoUnitTests boost_unit_test_framework  tomo Magick++ rt)

ADD_EXECUTABLE(TomoBenchmark ./src/test/Benchmark.cpp)
TARGET_LINK_LIBRARIES(TomoBenchmark tomo rt boost_system boost_timer)



#ADD_DEFINITIONS(-g -O2 -fsigned-char -freg-struct-return -Wall -W -Wshadow -Wpointer-arith -Wcast-qual -Winline)
#ADD_DEFINITIONS(-g -O3 -fsigned-char -fopenmp -freg-struct-return -Wall -fPIC)
ADD_DEFINITIONS(-g -O3 -fsigned-char -Wall -fPIC)

# add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)
IF(DOXYGEN_FOUND)
  CONFIGURE_FILE(${CMAKE_PROJECT_DOC_DIR}/tomo.doxygen ${CMAKE_PROJECT_DOC_DIR} @ONLY)
  ADD_CUSTOM_TARGET(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_PROJECT_DOC_DIR}/tomo.doxygen
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen (see ./doc/html/index.html)" VERBATIM
    )
ENDIF(DOXYGEN_FOUND)
