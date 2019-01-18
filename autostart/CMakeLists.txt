file(GLOB DESKTOP_FILES_IN *.desktop.in)

if (NOT APPLE)
    # Translations **********************************
    lxqt_translate_desktop(DESKTOP_FILES
        SOURCES
            ${DESKTOP_FILES_IN}
    )
endif()

add_custom_target(lxqt_notificationd_autostart_desktop_files ALL DEPENDS ${DESKTOP_FILES})
#************************************************

install(FILES
    ${DESKTOP_FILES}
    DESTINATION "${LXQT_ETC_XDG_DIR}/autostart"
    COMPONENT Runtime
)
