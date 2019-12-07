cmake_minimum_required(VERSION 3.5)

set(KF5_VERSION "5.65.0") # handled by release scripts
set(KF5_DEP_VERSION "5.65.0") # handled by release scripts
project(KDesignerPlugin VERSION ${KF5_VERSION})

include(FeatureSummary)
find_package(ECM 5.65.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)


set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)


set(REQUIRED_QT_VERSION 5.11.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)
find_package(Qt5Designer ${REQUIRED_QT_VERSION} NO_MODULE)
set_package_properties(Qt5Designer PROPERTIES
   PURPOSE "Required to build the Qt Designer plugins"
   TYPE OPTIONAL
)

include(ECMPoQmTools)

find_package(KF5CoreAddons ${KF5_DEP_VERSION} REQUIRED)
find_package(KF5Config ${KF5_DEP_VERSION} REQUIRED)
find_package(KF5DocTools ${KF5_DEP_VERSION})


set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5DesignerPlugin")
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050d00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x053f00)
if (KF5DocTools_FOUND)
    add_subdirectory(docs)
endif()
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ecm_install_po_files_as_qm(po)
    if (KF5DocTools_FOUND)
        kdoctools_install(po)
    endif()
endif()
add_definitions(-DQT_NO_FOREACH)
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
