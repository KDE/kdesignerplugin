include(ECMMarkNonGuiExecutable)

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
