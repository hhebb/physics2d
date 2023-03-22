# test build targets
cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_USE_QTOPENGL TRUE)
# cmake_policy(SET CMP0072 NEW)

# find library.
find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets OpenGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(jsoncpp REQUIRED)

# set sources for target.
set(PHYSICS_SOURCES
        main.cpp
        MainWindow.cpp
        window/RenderWindow.cpp
        apps/App.cpp
        physics/World.cpp
        physics/Body.cpp
        physics/Collider.cpp
        physics/Collision.cpp
        Helper.cpp
        physics/Joint.cpp
        physics/DistanceJoint.cpp
        physics/RevoluteJoint.cpp
        physics/PrismaticJoint.cpp
        physics/WorldParser.cpp
)

# add build target.
add_executable(physics ${PHYSICS_SOURCES})

# link founded libraries.
target_link_libraries(physics PRIVATE Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_link_libraries(physics PRIVATE GL)
target_link_libraries(physics PRIVATE GLU)
target_link_libraries(physics PRIVATE glut)
target_link_libraries(physics PRIVATE jsoncpp)


######################## for pybind ##############################

add_subdirectory(pybind11)

# set sources for bind extension.
set(BIND_SOURCES
        physics/Environment.cpp
        physics/Body.cpp
        physics/Collider.cpp
        physics/Collision.cpp
        physics/Joint.cpp
        physics/DistanceJoint.cpp
        physics/RevoluteJoint.cpp
        physics/PrismaticJoint.cpp
        physics/WorldParser.cpp
        Helper.cpp
)

# add extension
pybind11_add_module(env ${BIND_SOURCES})

# link founded libraries for pybind
target_link_libraries(env PRIVATE Qt5::Widgets Qt5::OpenGL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_link_libraries(env PRIVATE pybind11::module)
target_link_libraries(env PRIVATE jsoncpp)


