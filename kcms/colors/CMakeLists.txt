# KI18N Translation Domain for this library
add_definitions(-DTRANSLATION_DOMAIN=\"kcm_colors\")

set(kcm_colors_SRCS
    ../krdb/krdb.cpp
    colors.cpp
    colorsmodel.cpp
    filterproxymodel.cpp
)

# needed for krdb
qt5_add_dbus_interface(kcm_colors_SRCS ${KINIT_DBUS_INTERFACES_DIR}/kf5_org.kde.KLauncher.xml klauncher_iface)

add_library(kcm_colors MODULE ${kcm_colors_SRCS})
target_link_libraries(kcm_colors
    Qt5::DBus
    KF5::KCMUtils
    KF5::CoreAddons
    KF5::Declarative
    KF5::GuiAddons
    KF5::I18n
    KF5::KIOCore
    KF5::KIOWidgets
    KF5::NewStuff
    KF5::QuickAddons
    KF5::WindowSystem
)

if(X11_FOUND)
    target_link_libraries(kcm_colors ${X11_LIBRARIES} Qt5::X11Extras)
endif()

kcoreaddons_desktop_to_json(kcm_colors "kcm_colors.desktop")

install(FILES kcm_colors.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(TARGETS kcm_colors DESTINATION ${KDE_INSTALL_PLUGINDIR}/kcms)
install(FILES colorschemes.knsrc DESTINATION ${KDE_INSTALL_KNSRCDIR})

kpackage_install_package(package kcm_colors kcms)

# built-in color schemes
file(GLOB schemefiles schemes/*.colors)
install( FILES ${schemefiles} DESTINATION ${KDE_INSTALL_DATADIR}/color-schemes )

add_subdirectory(editor)
