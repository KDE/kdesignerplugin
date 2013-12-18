cmake_minimum_required(VERSION 2.8.12)

project(KF5DesignerPlugin)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)
include(FeatureSummary)

set(REQUIRED_QT_VERSION 5.2.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)
find_package(Qt5WebKitWidgets ${REQUIRED_QT_VERSION} NO_MODULE)
set_package_properties(Qt5WebKitWidgets PROPERTIES
   PURPOSE "Required to build the Qt Designer plugin for the KDEWebKit widgets"
   TYPE OPTIONAL
)
find_package(Qt5Designer ${REQUIRED_QT_VERSION} NO_MODULE)
set_package_properties(Qt5Designer PROPERTIES
   PURPOSE "Required to build the Qt Designer plugins"
   TYPE OPTIONAL
)
set(BUILD_KF5_DESIGNER_PLUGIN ${Qt5Designer_FOUND})
set(BUILD_KDEWEBKIT_DESIGNER_PLUGIN ${Qt5Designer_FOUND})

set(KF5_VERSION 5.0.0)
find_package(KF5CoreAddons ${KF5_VERSION} REQUIRED)
find_package(KF5Config ${KF5_VERSION} REQUIRED)
find_package(KF5DocTools ${KF5_VERSION} REQUIRED)

# Dependency hacks (private deps for things we actually need)
find_package(KF5GuiAddons ${KF5_VERSION})
find_package(KF5ConfigWidgets ${KF5_VERSION})
find_package(KF5Completion ${KF5_VERSION})
find_package(KF5DBusAddons ${KF5_VERSION})
find_package(KF5Service ${KF5_VERSION})
find_package(KF5WindowSystem ${KF5_VERSION})
find_package(KF5XmlGui ${KF5_VERSION})
find_package(KF5Bookmarks ${KF5_VERSION})

find_package(KF5I18n ${KF5_VERSION})
set_package_properties(KF5I18n PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")
set(BUILD_KF5_DESIGNER_PLUGIN ${BUILD_KF5_DESIGNER_PLUGIN} AND ${KF5I18n_FOUND})
find_package(KF5IconThemes ${KF5_VERSION})
set_package_properties(KF5IconThemes PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")
set(BUILD_KF5_DESIGNER_PLUGIN ${BUILD_KF5_DESIGNER_PLUGIN} AND ${KF5IconThemes_FOUND})
find_package(KF5ItemViews ${KF5_VERSION})
set_package_properties(KF5ItemViews PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")
find_package(KF5KIO ${KF5_VERSION})
set(BUILD_KF5_DESIGNER_PLUGIN ${BUILD_KF5_DESIGNER_PLUGIN} AND ${KF5ItemViews_FOUND})
set_package_properties(KF5KIO PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")
find_package(KF5Plotting ${KF5_VERSION})
set(BUILD_KF5_DESIGNER_PLUGIN ${BUILD_KF5_DESIGNER_PLUGIN} AND ${KF5KIO_FOUND})
set_package_properties(KF5Plotting PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")
set(BUILD_KF5_DESIGNER_PLUGIN ${BUILD_KF5_DESIGNER_PLUGIN} AND ${KF5Plotting_FOUND})
find_package(KF5TextWidgets ${KF5_VERSION})
set_package_properties(KF5TextWidgets PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")
set(BUILD_KF5_DESIGNER_PLUGIN ${BUILD_KF5_DESIGNER_PLUGIN} AND ${KF5TextWidgets_FOUND})
find_package(KF5WidgetsAddons ${KF5_VERSION})
set_package_properties(KF5WidgetsAddons PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")
set(BUILD_KF5_DESIGNER_PLUGIN ${BUILD_KF5_DESIGNER_PLUGIN} AND ${KF5WidgetsAddons_FOUND})
find_package(KF5WebKit ${KF5_VERSION})
set_package_properties(KF5WebKit PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for the KDEWebKit widgets")
set(BUILD_KDEWEBKIT_DESIGNER_PLUGIN ${BUILD_KDEWEBKIT_DESIGNER_PLUGIN} AND ${KF5WebKit_FOUND})

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5DesignerPlugin")

add_subdirectory(docs)
add_subdirectory(src)
add_subdirectory(tests)

include(ECMSetupVersion)
ecm_setup_version(${KF5_VERSION} VARIABLE_PREFIX kf5designerplugin
                                 VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kf5designerplugin_version.h"
                                 PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5DesignerPluginConfigVersion.cmake")

install( FILES
    KF5DesignerPluginMacros.cmake KF5DesignerPluginConfig.cmake "${CMAKE_CURRENT_BINARY_DIR}/KF5DesignerPluginConfigVersion.cmake"
    DESTINATION ${CMAKECONFIG_INSTALL_DIR} COMPONENT Devel )

install(EXPORT KF5DesignerPluginTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5DesignerPluginTargets.cmake NAMESPACE KF5:: )

if ("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
endif()
