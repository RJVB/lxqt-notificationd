cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
# CMP0000: Call the cmake_minimum_required() command at the beginning of the top-level
# CMakeLists.txt file even before calling the project() command.
# The cmake_minimum_required(VERSION) command implicitly invokes the cmake_policy(VERSION)
# command to specify that the current project code is written for the given range of CMake
# versions.
project(lxqt-notificationd)

set(CMAKE_MODULE_PATH ${lxqt-notificationd_SOURCE_DIR}/cmake)

include(GNUInstallDirs)
include(FeatureSummary)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(UPDATE_TRANSLATIONS "Update source translation translations/*.ts files" OFF)
option(LXQT_NOTIFICATION_BUILD_TESTS "Build LXQt Notification tests" OFF)
if (APPLE OR WIN32)
    option(STANDALONE "Build without LXQt (default on Mac and Windows)" ON)
else()
    option(STANDALONE "Build without LXQt (default on Mac and Windows)" OFF)
endif()

set(KF5_MINIMUM_VERSION "5.36.0")
set(LXQT_MINIMUM_VERSION "0.14.0")
set(QT_MINIMUM_VERSION "5.7.1")

find_package(Qt5DBus ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5LinguistTools ${QT_MINIMUM_VERSION} REQUIRED)
find_package(Qt5Widgets ${QT_MINIMUM_VERSION} REQUIRED)
find_package(KF5WindowSystem ${KF5_MINIMUM_VERSION} REQUIRED)
if (NOT STANDALONE)
    find_package(lxqt ${LXQT_MINIMUM_VERSION} REQUIRED)
else()
    include(CMakePackageConfigHelpers)
    include(GNUInstallDirs) # Standard directories for installation
    add_definitions(-DNOLXQT)
    set(NOLXQT ON)
endif()
message(STATUS "Building with Qt${Qt5Core_VERSION}")

# Patch Version
set(LXQT_NOTIFICATIOND_PATCH_VERSION 0)

if (LXQT_MAJOR_VERSION)
    set(LXQT_NOTIFICATIOND_VERSION ${LXQT_MAJOR_VERSION}.${LXQT_MINOR_VERSION}.${LXQT_NOTIFICATIOND_PATCH_VERSION})
endif()
if (NOLXQT)
    include(GetGitRevisionDescription)
    git_describe(GIT_FULL_VERSION "--tags")
    if(GIT_FULL_VERSION)
        # development build, i.e. from a git working copy.
        # Get the current commit version using `git describe` and
        # strip the leading v plus the part that matches the version
        # declared in the project definition above.
        string(REPLACE "v${LXQT_NOTIFICATIOND_VERSION}" "" COMMIT_VERSION "${GIT_FULL_VERSION}")
        if(NOT ${COMMIT_VERSION} MATCHES ${GIT_FULL_VERSION})
            # `git describe` agrees with LXQT_NOTIFICATIOND_VERSION;
            # make an atomic version string by appending the additional info
            string(APPEND LXQT_NOTIFICATIOND_VERSION ${COMMIT_VERSION})
        elseif(LXQT_MAJOR_VERSION)
            # `git describe` provides a seemingly unrelated version string;
            # append it in parentheses.
            string(APPEND LXQT_NOTIFICATIOND_VERSION " (${GIT_FULL_VERSION})")
        else()
            set(LXQT_NOTIFICATIOND_VERSION ${GIT_FULL_VERSION})
        endif()
        message(STATUS "Setting lxqt-notificationd version to: ${LXQT_NOTIFICATIOND_VERSION}")
    endif()
    configure_file(notifyd_fullversion.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/notifyd_fullversion.h)
endif()

add_definitions(
    "-DLXQT_NOTIFICATIOND_VERSION=\"${LXQT_NOTIFICATIOND_VERSION}\""
    "-DQT_NO_CAST_FROM_ASCII"
    "-DQT_NO_CAST_TO_ASCII"
    "-DQT_NO_URL_CAST_FROM_STRING"
    "-DQT_NO_CAST_FROM_BYTEARRAY"
)

if (NOT STANDALONE)
    include(LXQtPreventInSourceBuilds)
    include(LXQtCompilerSettings NO_POLICY_SCOPE)
    # Translations **********************************
    include(LXQtTranslate)
endif()

add_subdirectory(config)
add_subdirectory(src)
if (LXQT_NOTIFICATION_BUILD_TESTS AND NOT STANDALONE)
    add_subdirectory(test)
endif()

## merged from lxqt-common
add_subdirectory(autostart)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
