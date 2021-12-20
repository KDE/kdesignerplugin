remove_definitions(-DQT_NO_CAST_FROM_ASCII)

include(ECMAddTests)

find_package(Qt${QT_MAJOR_VERSION}Test ${REQUIRED_QT_VERSION} CONFIG QUIET)
find_package(Qt${QT_MAJOR_VERSION}Widgets ${REQUIRED_QT_VERSION} CONFIG QUIET)
if(NOT Qt5Designer_FOUND OR NOT Qt5Test_FOUND OR NOT Qt5Widgets_FOUND)
    message(STATUS "Autotests will not be built, as Qt5Designer, Qt5Widgets or Qt5Test was not found.")
    return()
endif()

include(${KDesignerPlugin_SOURCE_DIR}/KF5DesignerPluginMacros.cmake)

kf5designerplugin_add_plugin(testplugin testplugin.widgets testplugin.qrc)
target_link_libraries(testplugin PRIVATE Qt${QT_MAJOR_VERSION}::Widgets)
kf5designerplugin_add_plugin(minimalplugin minimal.widgets)
target_link_libraries(minimalplugin PRIVATE Qt${QT_MAJOR_VERSION}::Widgets)

macro(kdp_add_tests)
    foreach(f ${ARGN})
        get_filename_component(_testname "${f}" NAME_WE)
        ecm_add_test(${f}
            TEST_NAME ${_testname}
            LINK_LIBRARIES
                Qt${QT_MAJOR_VERSION}::Test
                Qt${QT_MAJOR_VERSION}::Widgets
        )
        target_include_directories(${_testname}
            PRIVATE
                ${Qt5Designer_INCLUDE_DIRS}
        )
        target_compile_definitions(${_testname} PRIVATE
            TEST_PLUGIN="$<TARGET_FILE:testplugin>"
            MINIMAL_PLUGIN="$<TARGET_FILE:minimalplugin>"
        )
    endforeach()
endmacro()

kdp_add_tests(
    plugintest.cpp
    minimaltest.cpp
)
