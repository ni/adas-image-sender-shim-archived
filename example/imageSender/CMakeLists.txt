cmake_minimum_required(VERSION 3.0.0)
project(ImageSenderDemo VERSION 0.1.0)

include(CTest)
enable_testing()

set(cmake_exe_com)

if (WIN32)
    set(Boost_USE_STATIC_LIBS ON)
else()
    set(GCC_COVERAGE_COMPILE_FLAGS  "-fPIC")
    set(CMAKE_CXX_FLAGS             "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
    set(GCC_COVERAGE_LINK_FLAGS     "-shared")
    set(CMAKE_EXE_LINKER_FLAGS      "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
endif()

aux_source_directory(source DIR_SRC)

set(HEADERS source)
file(GLOB_RECURSE SOURCE source/main.cpp)

include_directories(${HEADERS})
include_directories(ImageSender/includes)

add_executable(ImageSenderDemo ${SOURCE})

add_library(IMAGE_SENDER SHARED IMPORTED)
find_package(Boost REQUIRED COMPONENTS program_options)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 

    if(WIN32) 
        set_target_properties(IMAGE_SENDER PROPERTIES 
            IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/ImageSender/windows/ImageSenderLib/imageSender.lib" 
        )
    else()
        set_target_properties(IMAGE_SENDER PROPERTIES 
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/ImageSender/linux/ImageSenderLib/libimageSender.so" 
        )
    endif()

    target_link_libraries(ImageSenderDemo ${Boost_LIBRARIES})
    target_link_libraries(ImageSenderDemo IMAGE_SENDER)
else()
    message(STATUS "Failed find boost library!")
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
