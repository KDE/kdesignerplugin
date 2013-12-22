# KDesignerPlugin

## Introduction

This framework provides plugins for Qt Designer that allow it to display
the widgets provided by various KDE frameworks, as well as a utility
(kgendesignerplugin) that can be used to generate other such plugins
from ini-style description files.

## kgendesignerplugin

To use kgendesignerplugin in your own project, add
  find_package(KF5DesignerPlugin)
to your CMakeLists.txt file, and use the
kf5designerplugin\_add\_widget\_files macro.  For example, you might do

    find_package(Qt5Designer)
    find_package(KF5DesignerPlugin NO_MODULE)
    include_directories(${Qt5Designer_INCLUDE_DIRS})
    if(Qt5Designer_FOUND AND KF5DesignerPlugin_FOUND)
       set(foowidgets_SRCS)
       kf5designerplugin_add_widget_files(foowidgets_SRCS
          foo.widgets
       )
       qt5_add_resources(foowidgets_SRCS
          foowidgets.qrc # preview images
       )
       add_library(foowidgets MODULE ${foowidgets_SRCS})
       target_link_libraries(foowidgets
          foo # library containing the actual widget classes
       )
       install(TARGETS foowidgets DESTINATION ${QT_PLUGIN_INSTALL_DIR}/designer)
    endif()

foo.widgets is an ini-style configuration file (see the KConfig
framework for format details) describing the widgets exported by the
plugin.  It must include a [Global] section providing a PluginName entry
(the value of which must be a valid C++ class name).  For example,

    [Global]
    PluginName=FooWidgets

This section can also list additional headers that should be included
for the plugin code to work, for example preview classes (see ImplClass
documentation below), with the Includes entry:

    [Global]
    PluginName=FooWidgets
    Includes=classpreviews.h

In addition, it should have a section for each widget you wish to
include in the plugin.  For example, if you have a class FooView (that
inherits QWidget), whose header file is fooview.h, you could have the
section

    [FooView]
    IncludeFile=fooviewwidget.h
    ImplClass=FooViewPreview
    Group=Views (Foo)

The implementation of every method of QDesignerCustomWidgetInterface can
be controlled using options similar to the Group entry in the above
example, which causes the group() method to return "Views (Foo)".

kgendesignerplugin attempts to pick sensible defaults for these values:

- IncludeFile: would default to "fooview.h" if it were omitted from the
  example
- Group: defaults to the value of the -g option passed to
  kgendesignerplugin, which itself defaults to "Custom"
- ToolTip: defaults to "FooView Widget" in the example
- WhatsThis: defaults to "FooView Widget" in the example
- IsContainer: defaults to false
- CreateWidget: defaults to `return new ImplClass ConstructorArgs` where
  ImplClass and ConstructorArgs are entries (see below)
- DomXml: defaults to the value of
  QDesignerCustomWidgetInterface::domXml()
- Initialize: the default simply sets the mInitialized variable to true
  if it is not already

Note that no Icon entry is given above, instead you should provide an
icon name with IconName; this defaults to ":/pics/fooview.png" in the
above example.  You should include this image in a resource file.

If your class requires arguments other than the parent widget, or you
need to use a different class for previewing the widget in Qt Designer
(FooViewPreview in the above example), rather than setting CreateWidget
you can use some combination of ImplClass and ConstructorArgs:

- ImplClass: would default to FooView if it were omitted from the
  example
- ConstructorArgs: defaults to (parent) - note that the parentheses
  are required


## Links

- Mailing list: <https://mail.kde.org/mailman/listinfo/kde-frameworks-devel>
- IRC channel: #kde-devel on Freenode
- Git repository: <https://projects.kde.org/projects/frameworks/kdesignerplugin/repository>
