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
SET(SOURCE_TLIB Tiomanip.cpp Tiostream.cpp Tlocale.cpp Tstring.cpp Tcodecvt.cpp)

SET(TARGT_NAME_TPARSERS tlib_parsers)
SET(SOURCE_TPARSERS INIParser.cpp CLParser.cpp)

#SET(TARGT_NAME_TEXCEPTIONS tlib_exceptions)
#SET(SOURCE_TEXCEPTIONS TException.cpp)

SET(TARGT_NAME_TCHAR16 tlib_char16)
SET(SOURCE_TCHAR16 ./win/crt_codecvt_id.cpp ./win/crt_collate_id.cpp)

if (MSVC)

    message(STATUS "MSVC")

    get_lib_suffix()    
    SET(TARGT_NAME_TLIB         ${TARGT_NAME_TLIB}${LIB_SUFFIX})
    SET(TARGT_NAME_TPARSERS     ${TARGT_NAME_TPARSERS}${LIB_SUFFIX})
#    SET(TARGT_NAME_TEXCEPTIONS  ${TARGT_NAME_TEXCEPTIONS}${LIB_SUFFIX})
    SET(TARGT_NAME_TCHAR16     ${TARGT_NAME_TCHAR16}${LIB_SUFFIX})    
    
    add_library(${TARGT_NAME_TLIB}        ${SOURCE_TLIB} "./win/locale_char16_t.cpp")    
    add_library(${TARGT_NAME_TPARSERS}    ${SOURCE_TPARSERS})    
#    add_library(${TARGT_NAME_TEXCEPTIONS} ${SOURCE_TEXCEPTIONS})    
    add_library(${TARGT_NAME_TCHAR16} ${SHARED_LIB} ${SOURCE_TCHAR16})    

        
    target_include_directories(${TARGT_NAME_TLIB} PUBLIC "./win/" )
    target_include_directories(${TARGT_NAME_TPARSERS} PUBLIC "./win/" )
#    target_include_directories(${TARGT_NAME_TEXCEPTIONS} PUBLIC "./win/" )
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

    add_library(${TARGT_NAME_TLIB}        ${SOURCE_TLIB} "./linux/locale_char16_t.cpp")
    add_library(${TARGT_NAME_TPARSERS}    ${SOURCE_TPARSERS})      
    #add_library(${TARGT_NAME_TEXCEPTIONS} ${SOURCE_TEXCEPTIONS})    

    target_include_directories(${TARGT_NAME_TLIB} PUBLIC "./linux/" )    
    target_include_directories(${TARGT_NAME_TPARSERS} PUBLIC "./linux/" )    
    #target_include_directories(${TARGT_NAME_TEXCEPTIONS} PUBLIC "./linux/" )    

	set_target_properties(${TARGT_NAME_TLIB} ${TARGT_NAME_TPARSERS} PROPERTIES
		  CXX_STANDARD 17
		  CXX_STANDARD_REQUIRED ON
		  COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
	)

    add_compile_definitions(_LIB)
    add_compile_definitions(__linux__)
    
endif ()

target_include_directories(${TARGT_NAME_TLIB} PUBLIC "../include/" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.." )
target_include_directories(${TARGT_NAME_TPARSERS} PUBLIC "../include/" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.." )
#target_include_directories(${TARGT_NAME_TEXCEPTIONS} PUBLIC "../include/" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/.." )



#set_target_properties(${TARGT_NAME} PROPERTIES PUBLIC_HEADER "async.h;shared_EXPORTS.h;../include/exception_list.h")

#install(TARGETS ${TARGT_NAME}
#        LIBRARY
#          DESTINATION lib
#        PUBLIC_HEADER
#          DESTINATION include
#       )