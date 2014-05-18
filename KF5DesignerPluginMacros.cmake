# kf5designerplugin_generate_plugin(<sources_var> <widgetsfile>)
#
# Generates the necessary source code files for a Qt Designer widget plugin
# from the given ini-style description file, and adds them to <sources_var>.
# Note that plugins cannot be combined: you cannot add the generated code files
# from more than one widgets file into the same plugin.
#
# Usage:
#
#   set(foo_SRCS otherfile.cpp)
#   qt5_add_resources(foo_SRCS widgeticons.qrc)
#   kf5designerplugin_generate_plugin(foo_SRCS foo.widgets)
#   add_library(foowidgets MODULE ${foo_SRCS})
#
#
# kf5designerplugin_add_plugin(<target> [OPTIONAL]
#                              <sourcefile> [<sourcefile> [...]])
#
# This behaves like add_library() (with the MODULE argument), except that if
# one of the source files has the extension ".widgets", kgendesignerplugin will
# be used to generate Qt Designer plugin source files from it. Additionally,
# any source files with the extension ".qrc" will be dealt with appropriately
# as well.
#
# The OPTIONAL argument specifies that if Qt Designer was not found, the macro
# should just not create the target instead of causing an error. You should
# then use "if(TARGET <target>)" to wrap anything that makes use of the target.
#
# The usage example for kf5designerplugin_generate_plugin can be replaced with
#
#   kf5designerplugin_add_plugin(foowidgets
#       foo.widgets otherfile.cpp widgeticons.qrc)
#
# but you still need to link against the appropriate libraries (the ones that
# provide the widgets described in the plugin) and install the plugin to the
# "designer" subdirectory of a directory Qt will look in for plugins (see
# QPluginLoader).
#
# Note that only one .widgets file can be provided to each call to
# kf5designerplugin_add_plugin.

# Copyright (c) 2006-2009 Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2006, 2007, Laurent Montel, <montel@kde.org>
# Copyright (c) 2007 Matthias Kretz <kretz@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

function(kf5designerplugin_generate_plugin outvar widgetfile)
    get_filename_component(input ${widgetfile} ABSOLUTE)
    get_filename_component(basename ${input} NAME_WE)
    set(source ${CMAKE_CURRENT_BINARY_DIR}/${basename}widgets.cpp)

    # create source file from the .widgets file
    add_custom_command(OUTPUT ${source}
        COMMAND KF5::kgendesignerplugin
        ARGS -o ${source} ${input}
        MAIN_DEPENDENCY ${input})

    # create moc file
    set(moc ${CMAKE_CURRENT_BINARY_DIR}/${basename}widgets.moc)
    qt5_generate_moc(${source} ${moc} ${ARGN})

    set(${outvar} ${${outvar}} "${source}" "${moc}" PARENT_SCOPE)
endfunction()

# This needs to be a macro because of the nested find_package call
# (which will set some variables).
macro(kf5designerplugin_add_plugin target)
    set(_requiredarg REQUIRED)
    set(_files)
    foreach(f ${ARGN})
        if(f MATCHES "\\.widgets$")
            kf5designerplugin_generate_plugin(_files "${f}" TARGET "${target}")
        elseif(f MATCHES "\\.qrc$")
            qt5_add_resources(_files "${f}")
        elseif(f MATCHES "OPTIONAL")
            set(_requiredarg)
        else()
            list(APPEND _files "${f}")
        endif()
    endforeach()

    if(NOT Qt5Designer_FOUND)
        find_package(Qt5Designer ${REQUIRED_QT_VERSION} ${_requiredarg} NO_MODULE)
        set_package_properties(Qt5Designer PROPERTIES
            PURPOSE "Required to build Qt Designer plugins"
        )
    endif()
    if(Qt5Designer_FOUND)
        add_library(${target} MODULE ${_files})
        target_include_directories(${target} PRIVATE ${Qt5Designer_INCLUDE_DIRS})
    endif()
endmacro()

