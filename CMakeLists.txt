# cmake requirements
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# project id
PROJECT(Tomo)

# force outut into several folders
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
SET_TARGET_PROPERTIES( OpenMeshCore PROPERTIES COMPILE_FLAGS " -pipe -O3 -DINCLUDE_TEMPLATES -W -Wall -Wno-unused -DNDEBUG -D_REENTRANT -Wall -W -fPIC " )


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

#Tests
ADD_EXECUTABLE(mediantest ./src/test/mediantest.cpp)
TARGET_LINK_LIBRARIES(mediantest boost_program_options tomo)

ADD_EXECUTABLE(pointCloudTest ./src/test/pointCloudTest.cpp)
TARGET_LINK_LIBRARIES(pointCloudTest boost_program_options tomo rt)

ADD_EXECUTABLE(VectorTest ./src/test/VectorTest.cpp)
TARGET_LINK_LIBRARIES(VectorTest boost_program_options rt)

ADD_EXECUTABLE(MeshTest ./src/test/MeshTest.cpp)
TARGET_LINK_LIBRARIES(MeshTest tomo boost_program_options rt)

ADD_EXECUTABLE(PolygonTest ./src/test/PolygonTest.cpp)
TARGET_LINK_LIBRARIES(PolygonTest tomo boost_program_options rt Magick++)

ADD_EXECUTABLE(CompoundTest ./src/test/CompoundTest.cpp)
TARGET_LINK_LIBRARIES(CompoundTest tomo boost_program_options rt Magick++)

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
