cmake_minimum_required(VERSION 3.14)
project(PointGenerator)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    main.cpp
    src/ProjectionEngine.cpp  
    src/ProjectionManager.cpp  
    src/Point.cpp   
    src/RandomPointGenerator.cpp
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/external
)

find_package(PROJ REQUIRED)
if(PROJ_FOUND)
    message(STATUS "Found PROJ: ${PROJ_LIBRARIES}")
    include_directories(${PROJ_INCLUDE_DIRS})
endif()

add_executable(main ${SOURCES})

target_link_libraries(main PRIVATE ${PROJ_LIBRARIES})
