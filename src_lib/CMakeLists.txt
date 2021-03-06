cmake_minimum_required(VERSION 3.2)

if(STATIC_LINK_LIBS)
        message(STATUS "CMake STATIC_LINK_LIBS = ${STATIC_LINK_LIBS}")            
        if (MSVC)
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL ${CMAKE_CXX_FLAGS_MINSIZEREL})
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
            string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
        endif ()
else ()
        SET(SHARED_LIB SHARED)
endif ()


SET(TARGT_NAME_TLIB tlib)
SET(SOURCE_TLIB Tstring.cpp Tiostream.cpp Tlocale.cpp 
                Tcodecvt.cpp Tiomanip.cpp Tenvironment.cpp Tlib_version.cpp
                "./locale/locale_name.cpp"
                "./locale/code_page.cpp"
                "./locale/Tlocale_class.cpp")

SET(TARGT_NAME_TPARSERS tlib_parsers)
SET(SOURCE_TPARSERS INIParser.cpp 
                   "./clparser/Tcl_parser_impl.cpp"
                   "./clparser/Tcl_parser_interface_impl.cpp")

SET(TARGT_NAME_TCHAR16 tlib_char16)
SET(SOURCE_TCHAR16 ./win/crt_facets_id_codecvt.cpp ./win/crt_facets_id.cpp)

if (MSVC)

    message(STATUS "MSVC")

    get_lib_suffix()    
    SET(TARGT_NAME_TLIB         ${TARGT_NAME_TLIB}${LIB_SUFFIX})
    SET(TARGT_NAME_TPARSERS     ${TARGT_NAME_TPARSERS}${LIB_SUFFIX})
    SET(TARGT_NAME_TCHAR16     ${TARGT_NAME_TCHAR16}${LIB_SUFFIX})    
    
    add_library(${TARGT_NAME_TLIB}        ${SOURCE_TLIB} "./win/exception_win.cpp" "./win/locale_mbrlen_win.cpp")    
    add_library(${TARGT_NAME_TPARSERS}    ${SOURCE_TPARSERS})    
    add_library(${TARGT_NAME_TCHAR16} ${SHARED_LIB} ${SOURCE_TCHAR16})    
        
    target_include_directories(${TARGT_NAME_TLIB} PUBLIC "./win/" )
    target_include_directories(${TARGT_NAME_TPARSERS} PUBLIC "./win/" )
    target_include_directories(${TARGT_NAME_TCHAR16} PUBLIC  "./win/" "../include/" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.." )

	set_target_properties(${TARGT_NAME_TLIB} ${TARGT_NAME_TPARSERS} ${TARGT_NAME_TCHAR16} PROPERTIES
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      COMPILE_OPTIONS "/permissive-;/Zc:wchar_t"
    )

    add_compile_definitions(WIN32)
    add_compile_definitions(_LIB)
    add_compile_definitions(_UNICODE)
    add_compile_definitions(UNICODE)

else()

    add_library(${TARGT_NAME_TLIB}        ${SOURCE_TLIB} "./linux/locale_facets_linux.cpp" "./linux/locale_mbrlen_linux.cpp")
    add_library(${TARGT_NAME_TPARSERS}    ${SOURCE_TPARSERS})      

    target_include_directories(${TARGT_NAME_TLIB} PUBLIC "./linux/" )    
    target_include_directories(${TARGT_NAME_TPARSERS} PUBLIC "./linux/" )    

	set_target_properties(${TARGT_NAME_TLIB} ${TARGT_NAME_TPARSERS} PROPERTIES
		  CXX_STANDARD 17
		  CXX_STANDARD_REQUIRED ON
		  COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-g;-o0"
	)

    add_compile_definitions(_LIB)
    add_compile_definitions(__linux__)

    target_link_libraries(${TARGT_NAME_TPARSERS} stdc++fs)
    
endif ()

target_include_directories(${TARGT_NAME_TLIB} PUBLIC "../include/" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.." )
target_include_directories(${TARGT_NAME_TPARSERS} PUBLIC "../include/" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.." )


#set_target_properties(${TARGT_NAME} PROPERTIES PUBLIC_HEADER "async.h;shared_EXPORTS.h;../include/exception_list.h")

#install(TARGETS ${TARGT_NAME}
#        LIBRARY
#          DESTINATION lib
#        PUBLIC_HEADER
#          DESTINATION include
#       )