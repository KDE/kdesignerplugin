include(ECMMarkNonGuiExecutable)

add_executable(kgendesignerplugin)
add_executable(KF5::kgendesignerplugin ALIAS kgendesignerplugin)

ecm_create_qm_loader(kgendesignerplugin kdesignerplugin5_qt)

target_sources(kgendesignerplugin PRIVATE
  kgendesignerplugin.cpp
)
ecm_mark_nongui_executable(kgendesignerplugin)

target_link_libraries(kgendesignerplugin KF5::ConfigCore # KConfig + KConfigGroup
                                         KF5::CoreAddons # KMacroExpander
                                         )

install(TARGETS kgendesignerplugin  EXPORT KF5DesignerPluginTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS} )
