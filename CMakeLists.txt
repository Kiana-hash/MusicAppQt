cmake_minimum_required(VERSION 3.19)
project(spotifymahroom LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(spotifymahroom
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    account.h account.cpp
    album.h album.cpp
    song.h song.cpp
    playlist.h playlist.cpp
    AbstractRepository.h
    songrepository.h songrepository.cpp
    RepositoryData.h
    playlistrepository.h playlistrepository.cpp
    albumrepository.h albumrepository.cpp
    accountrepository.h accountrepository.cpp
    artistrepository.h artistrepository.cpp
    listenerrepository.h listenerrepository.cpp
    authservice.h authservice.cpp
    artistservice.h artistservice.cpp
    listenerservice.h listenerservice.cpp
    accountservice.h accountservice.cpp
    musicqueryservice.h musicqueryservice.cpp
    catalogservice.h catalogservice.cpp
    datastorage.h datastorage.cpp
    registerdialog.h registerdialog.cpp registerdialog.ui
    artistdashboard.h artistdashboard.cpp artistdashboard.ui
    listenerdashboard.h listenerdashboard.cpp listenerdashboard.ui
    createalbumdialog.h createalbumdialog.cpp createalbumdialog.ui
    albumsongsdialog.h albumsongsdialog.cpp albumsongsdialog.ui
    createsongdialog.h createsongdialog.cpp createsongdialog.ui
    editsongdialog.h editsongdialog.cpp editsongdialog.ui
    editalbumdialog.h editalbumdialog.cpp editalbumdialog.ui
    artistbrowsedialog.h artistbrowsedialog.cpp artistbrowsedialog.ui
    listenersongsdialog.h listenersongsdialog.cpp listenersongsdialog.ui
    chooseplaylistdialog.h chooseplaylistdialog.cpp chooseplaylistdialog.ui
    createplaylistdialog.h createplaylistdialog.cpp createplaylistdialog.ui
    playlistsongsdialog.h playlistsongsdialog.cpp playlistsongsdialog.ui
    accountsettingsdialog.h accountsettingsdialog.cpp accountsettingsdialog.ui

)

target_link_libraries(spotifymahroom
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS spotifymahroom
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET spotifymahroom
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
