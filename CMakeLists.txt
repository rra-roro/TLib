cmake_minimum_required(VERSION 3.2)

if (NOT TRAVIS_BUILD_NUMBER)
    set(TRAVIS_BUILD_NUMBER $ENV{TRAVIS_BUILD_NUMBER})
endif()

project(tlib VERSION 0.0.${TRAVIS_BUILD_NUMBER})

configure_file("./include/version.h.in" version.h)

include(./functions.cmake)

add_subdirectory(src_lib)
add_subdirectory(src_test)
add_subdirectory(src_examples)

set(CNCC_PATH $ENV{CNCC_PATH})

if (MSVC AND CNCC_PATH)

        file(GLOB_RECURSE MY_SOURCE_FILES "*.cpp" "*.h" "*.c" "*.hpp")
        add_custom_target(${CMAKE_PROJECT_NAME}.analyzeCNCC
                            COMMAND python ${CNCC_PATH}/cncc --style=${PROJECT_SOURCE_DIR}/cncc.style  ${MY_SOURCE_FILES}
                            DEPENDS ${ALL_SOURCE}
                            VERBATIM
        )
        set_target_properties(${CMAKE_PROJECT_NAME}.analyzeCNCC PROPERTIES EXCLUDE_FROM_ALL TRUE)

endif()

install(TARGETS)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT rra.roro@gmail.com)

include(CPack)

enable_testing()
add_test(test_tlib src_test/test_tlib)