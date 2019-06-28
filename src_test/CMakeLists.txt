cmake_minimum_required(VERSION 3.2)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.12")
    message(STATUS "CMake version ${CMAKE_VERSION}")
    cmake_policy(SET CMP0074 NEW)
endif ()

if(STATIC_LINK_LIBS)
        message(STATUS "CMake STATIC_LINK_LIBS = ${STATIC_LINK_LIBS}")
        if (MSVC)
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_MINSIZEREL})
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
            string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
        endif ()
        set(GTEST_LINKED_AS_SHARED_LIBRARY 0)
endif ()

enable_testing()
find_package(GTest REQUIRED)

message(STATUS "GTest include: ${GTEST_INCLUDE_DIR}")

SET(TARGT_NAME test_tlib)
add_executable(${TARGT_NAME}  test_tlib.cpp)

target_include_directories(${TARGT_NAME}  PUBLIC "../include/" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.." ${GTEST_INCLUDE_DIR})


if (MSVC)
        get_lib_suffix() 
        SET(ALL_LIBS GTest::GTest GTest::Main tlib_parsers${LIB_SUFFIX} tlib${LIB_SUFFIX} tlib_char16${LIB_SUFFIX})

        set_target_properties(${TARGT_NAME}  PROPERTIES
          CXX_STANDARD 17
          CXX_STANDARD_REQUIRED ON
          COMPILE_OPTIONS "/permissive-;/Zc:wchar_t"
        )

        add_compile_definitions(WIN32)
        add_compile_definitions(_LIB)
        add_compile_definitions(_UNICODE)
        add_compile_definitions(UNICODE)


else()

        SET(ALL_LIBS GTest::GTest GTest::Main tlib_parsers tlib pthread)
        #SET(Linux_LIBS pthread stdc++fs)

        set_target_properties(${TARGT_NAME}  PROPERTIES
                  CXX_STANDARD 17
                  CXX_STANDARD_REQUIRED ON
                  COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
        )

        add_compile_definitions(__linux__)

endif ()

target_link_libraries(${TARGT_NAME} ${ALL_LIBS})

