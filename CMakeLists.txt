CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Tomo)
FIND_PACKAGE(Qt4 REQUIRED)

INCLUDE_DIRECTORIES(./include ./TbdLib/include)

AUX_SOURCE_DIRECTORY(./src/lib tomo_src)

ADD_LIBRARY(tomo STATIC ${tomo_src})
TARGET_LINK_LIBRARIES(tomo glut GLU GL)

INCLUDE(${QT_USE_FILE})
SET(cg2ex1_SOURCES ./src/cg2ex1/glwidget.cpp ./src/cg2ex1/main.cpp ./src/cg2ex1/MainWindow.cpp)
SET(cg2ex1_HEADERS ./src/cg2ex1/glwidget.h ./src/cg2ex1/MainWindow.h)
QT4_WRAP_CPP(cg2ex1_HEADERS_MOC ${cg2ex1_HEADERS})

ADD_EXECUTABLE(cg2ex1 ${cg2ex1_SOURCES} ${cg2ex1_HEADERS_MOC})
TARGET_LINK_LIBRARIES(cg2ex1 tomo -lQtOpenGL -lQtGui -lQtCore -lGLU -lGL -lpthread )

#Tests
ADD_EXECUTABLE(mediantest ./src/test/mediantest.cpp)
TARGET_LINK_LIBRARIES(mediantest boost_program_options tomo)

ADD_EXECUTABLE(pointCloudTest ./src/test/pointCloudTest.cpp)
TARGET_LINK_LIBRARIES(pointCloudTest boost_program_options tomo)

ADD_EXECUTABLE(VectorTest ./src/test/VectorTest.cpp)
TARGET_LINK_LIBRARIES(VectorTest boost_program_options)

#ADD_DEFINITIONS(-g -O2 -fsigned-char -freg-struct-return -Wall -W -Wshadow -Wpointer-arith -Wcast-qual -Winline)

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-g -O3 -fsigned-char -fopenmp -freg-struct-return -Wall -fPIC)

