cmake_minimum_required(VERSION 3.0)

set(KF5_VERSION "5.37.0") # handled by release scripts
set(KF5_DEP_VERSION "5.37.0") # handled by release scripts
project(KDesignerPlugin VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.38.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)


set(REQUIRED_QT_VERSION 5.7.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)
find_package(Qt5Designer ${REQUIRED_QT_VERSION} NO_MODULE)
set_package_properties(Qt5Designer PROPERTIES
   PURPOSE "Required to build the Qt Designer plugins"
   TYPE OPTIONAL
)

include(ECMPoQmTools)

find_package(KF5CoreAddons ${KF5_DEP_VERSION} REQUIRED)
find_package(KF5Config ${KF5_DEP_VERSION} REQUIRED)
find_package(KF5DocTools ${KF5_DEP_VERSION} REQUIRED)

find_package(KF5Completion ${KF5_DEP_VERSION})
set_package_properties(KF5Completion PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5ConfigWidgets ${KF5_DEP_VERSION})
set_package_properties(KF5ConfigWidgets PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5IconThemes ${KF5_DEP_VERSION})
set_package_properties(KF5IconThemes PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5KIO ${KF5_DEP_VERSION})
set_package_properties(KF5KIO PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5ItemViews ${KF5_DEP_VERSION})
set_package_properties(KF5ItemViews PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5Plotting ${KF5_DEP_VERSION})
set_package_properties(KF5Plotting PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5TextWidgets ${KF5_DEP_VERSION})
set_package_properties(KF5TextWidgets PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5WidgetsAddons ${KF5_DEP_VERSION})
set_package_properties(KF5WidgetsAddons PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5XmlGui ${KF5_DEP_VERSION})
set_package_properties(KF5XmlGui PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5Sonnet ${KF5_DEP_VERSION})
set_package_properties(KF5Sonnet PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for KDE Frameworks widgets")

find_package(KF5WebKit ${KF5_DEP_VERSION})
set_package_properties(KF5WebKit PROPERTIES TYPE OPTIONAL
   PURPOSE "Required to build the Qt Designer plugin for the KDEWebKit widgets")

if(Qt5Designer_FOUND AND KF5Completion_FOUND AND KF5ConfigWidgets_FOUND AND
  KF5IconThemes_FOUND AND KF5KIO_FOUND AND KF5ItemViews_FOUND AND KF5Plotting_FOUND
  AND KF5TextWidgets_FOUND AND KF5WidgetsAddons_FOUND AND KF5XmlGui_FOUND AND KF5Sonnet_FOUND)
    set(BUILD_KF5_DESIGNER_PLUGIN TRUE)
endif()

if(Qt5Designer_FOUND AND KF5WebKit_FOUND)
  set(BUILD_KDEWEBKIT_DESIGNER_PLUGIN TRUE)
endif()

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5DesignerPlugin")

add_subdirectory(docs)
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
    kdoctools_install(po)
endif()

add_subdirectory(src)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()

include(ECMSetupVersion)
ecm_setup_version(PROJECT VARIABLE_PREFIX kf5designerplugin
                                 VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kf5designerplugin_version.h"
                                 PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5DesignerPluginConfigVersion.cmake"
                                 SOVERSION 5)

install( FILES
    KF5DesignerPluginMacros.cmake KF5DesignerPluginConfig.cmake "${CMAKE_CURRENT_BINARY_DIR}/KF5DesignerPluginConfigVersion.cmake"
    DESTINATION ${CMAKECONFIG_INSTALL_DIR} COMPONENT Devel )

install(EXPORT KF5DesignerPluginTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE KF5DesignerPluginTargets.cmake NAMESPACE KF5:: )

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
