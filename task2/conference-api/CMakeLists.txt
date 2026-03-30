cmake_minimum_required(VERSION 3.12...3.31)
project(conference_api CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)

find_package(
    userver
    COMPONENTS core
    QUIET
)
if(NOT userver_FOUND)
    download_userver(TRY_DIR third_party/userver)
endif()

userver_setup_environment()

include_directories(src)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/storage/in_memory_storage.cpp
    src/auth/auth_utils.cpp
    src/handlers/register_user_handler.cpp
    src/handlers/login_handler.cpp
    src/handlers/get_user_by_login_handler.cpp
    src/handlers/search_users_handler.cpp
    src/handlers/create_talk_handler.cpp
    src/handlers/get_all_talks_handler.cpp
    src/handlers/create_conference_handler.cpp
    src/handlers/add_talk_to_conference_handler.cpp
    src/handlers/get_conference_talks_handler.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE userver::core
)

userver_testsuite_add_simple()

include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

file(GLOB CONFIGS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml
    ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${PROJECT_NAME}
)

install(
    FILES ${CONFIGS_FILES}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)