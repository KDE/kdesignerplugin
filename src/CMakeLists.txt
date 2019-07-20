include(ECMMarkNonGuiExecutable)

add_subdirectory( pics )

########### next target ###############

ecm_create_qm_loader(kdesignerplugin_QM_LOADER kdesignerplugin5_qt)

set(kgendesignerplugin_SRCS
  kgendesignerplugin.cpp
  ${kdesignerplugin_QM_LOADER}
)
add_executable(kgendesignerplugin ${kgendesignerplugin_SRCS})
add_executable(KF5::kgendesignerplugin ALIAS kgendesignerplugin)
ecm_mark_nongui_executable(kgendesignerplugin)

target_link_libraries(kgendesignerplugin KF5::ConfigCore # KConfig + KConfigGroup
                                         KF5::CoreAddons # KMacroExpander
                                         )

install(TARGETS kgendesignerplugin  EXPORT KF5DesignerPluginTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS} )


########### next target ###############

if(${Qt5Designer_FOUND})
    include(${KDesignerPlugin_SOURCE_DIR}/KF5DesignerPluginMacros.cmake)
endif()

if(BUILD_KF5_DESIGNER_PLUGIN)
    kf5designerplugin_add_plugin(kf5widgets kde.widgets kdewidgets.qrc)

    target_link_libraries(kf5widgets
        KF5::Completion
        KF5::ConfigWidgets
        KF5::IconThemes
        KF5::KIOFileWidgets
        KF5::KIOWidgets
        KF5::ItemViews
        KF5::Plotting
        KF5::TextWidgets
        KF5::WidgetsAddons
        KF5::XmlGui
        KF5::SonnetCore
    )
    if(NOT WIN32)
        # Since there are no libraries provided by this module,
        # there is no point including the build tree in RPath,
        # and then having to edit it at install time.
        set_target_properties(kf5widgets PROPERTIES
            SKIP_BUILD_RPATH TRUE
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif()

    install(TARGETS kf5widgets DESTINATION ${KDE_INSTALL_QTPLUGINDIR}/designer)
endif()
