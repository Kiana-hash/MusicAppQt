#include "mainwindow.h"

#include "RepositoryData.h"
#include "datastorage.h"

#include "artistrepository.h"
#include "listenerrepository.h"
#include "albumrepository.h"
#include "songrepository.h"
#include "playlistrepository.h"

#include "authservice.h"
#include "artistservice.h"
#include "listenerservice.h"
#include "accountservice.h"
#include "catalogservice.h"

#include <QApplication>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    RepositoryData data;

    DataStorage storage("data/database.txt");

    if (!storage.loadAll(data))
    {
        QMessageBox::critical(nullptr,"Data Error",
            "The saved application data could not be loaded."
            );

        return 1;
    }

    ArtistRepository artistRepository(data);

    ListenerRepository listenerRepository(data);

    AlbumRepository albumRepository(data);

    SongRepository songRepository(data);

    PlaylistRepository playlistRepository(data);

    AuthService authService(
        artistRepository,
        listenerRepository,
        playlistRepository
        );


    ArtistService artistService(
        artistRepository,
        albumRepository,
        songRepository,
        playlistRepository
        );


    ListenerService listenerService(
        listenerRepository,
        playlistRepository,
        songRepository
        );


    CatalogService catalogService(
        artistRepository,
        albumRepository,
        songRepository
        );


    AccountService accountService(
        artistRepository,
        listenerRepository,
        albumRepository,
        songRepository,
        playlistRepository,
        artistService
        );


    MainWindow w(
        authService,
        artistService,
        listenerService,
        catalogService,
        accountService
        );

    w.show();

    const int applicationResult =a.exec();


    if (!storage.saveAll(data))
    {

        return 1;
    }


    return applicationResult;
}