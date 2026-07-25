#include "listenerdashboard.h"
#include "ui_listenerdashboard.h"
#include "listenerservice.h"
#include "catalogservice.h"
#include "accountservice.h"
#include "artistbrowsedialog.h"
#include "createplaylistdialog.h"
#include "playlistsongsdialog.h"
#include "accountsettingsdialog.h"
#include "chooseplaylistdialog.h"
#include "musicplayerdialog.h"
#include "musicqueryservice.h"
#include "imageutils.h"

#include <QComboBox>
#include <QHeaderView>
#include <QIcon>
#include <QLineEdit>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <set>


ListenerDashboard::ListenerDashboard(
    int listenerId,
    ListenerService& listenerService,
    CatalogService& catalogService,
    AccountService& accountService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::ListenerDashboard),
    m_listenerId(listenerId),
    m_listenerService(listenerService),
    m_catalogService(catalogService),
    m_accountService(accountService)
{
    ui->setupUi(this);

    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->songsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->songsTableWidget->verticalHeader()->setVisible(false);

    ui->songsTableWidget->setIconSize(QSize(40, 40));

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);

    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);

    setupFilterOptions();

    connect(ui->searchLineEdit,&QLineEdit::textChanged,this,&ListenerDashboard::refreshSongs);

    connect(ui->genreComboBox,&QComboBox::currentTextChanged,this,&ListenerDashboard::refreshSongs);

    connect(ui->yearComboBox,&QComboBox::currentTextChanged,this,&ListenerDashboard::refreshSongs);

    connect(ui->sortComboBox,&QComboBox::currentTextChanged,this,&ListenerDashboard::refreshSongs);

    connect(ui->songsTableWidget,&QTableWidget::itemSelectionChanged,this,&ListenerDashboard::updateSongButtons);

    refreshDashboard();
}


ListenerDashboard::~ListenerDashboard()
{
    delete ui;
}

vector<Song> ListenerDashboard::getAllCatalogSongs() const
{
    vector<Song> songs;

    const vector<Account> artists =m_catalogService.getAllArtists();

    for (const Account& artist : artists)
    {

        const vector<Song> singles =m_catalogService.getArtistSingles(artist.getId());

        for (const Song& song : singles)
        {
            songs.push_back(song);
        }

        const vector<Album> albums =m_catalogService.getArtistAlbums(artist.getId());

        for (const Album& album : albums)
        {
            const vector<Song> albumSongs =m_catalogService.getAlbumSongs(album.getId());

            for (const Song& song : albumSongs)
            {
                songs.push_back(song);
            }
        }
    }

    return songs;
}

void ListenerDashboard::setupFilterOptions()
{
    const vector<Song> songs =getAllCatalogSongs();

    set<string> genres;
    set<int> years;

    for (const Song& song : songs)
    {
        genres.insert(song.getGenre());

        years.insert( song.getReleaseYear());
    }

    ui->genreComboBox->clear();

    ui->genreComboBox->addItem("All Genres");

    for (const string& genre : genres)
    {
        ui->genreComboBox->addItem(QString::fromStdString(genre));
    }

    ui->yearComboBox->clear();

    ui->yearComboBox->addItem("All Years");

    for (int year : years)
    {
        ui->yearComboBox->addItem(QString::number(year));
    }

    ui->sortComboBox->clear();

    ui->sortComboBox->addItem("Name A-Z");

    ui->sortComboBox->addItem("Name Z-A");

    ui->sortComboBox->addItem("Year Newest-Oldest");

    ui->sortComboBox->addItem("Year Oldest-Newest");
}

void ListenerDashboard::refreshDashboard()
{
    const optional<Account> account =m_accountService.getAccount(m_listenerId);


    if (!account.has_value())
    {
        return;
    }

    ui->welcomeLabel->setText("Welcome, " + QString::fromStdString(account->getFullName()));

    ImageUtils::displayImage(ui->profilePhotoLabel,QString::fromStdString(account->getProfilePhotoPath()));

    ui->artistsListWidget->clear();

    const vector<Account> artists =m_catalogService.getAllArtists();

    for (const Account& artist : artists)
    {
        QListWidgetItem* item =new QListWidgetItem(QString::fromStdString(artist.getFullName()));

        item->setData(Qt::UserRole,artist.getId());

        const QString photoPath =QString::fromStdString(artist.getProfilePhotoPath());

        if (!photoPath.isEmpty())
        {
            item->setIcon(QIcon(photoPath));
        }

        ui->artistsListWidget->addItem(item);
    }

    ui->playlistsListWidget->clear();

    const vector<Playlist> playlists =m_listenerService.getPlaylists(m_listenerId);

    for (const Playlist& playlist : playlists)
    {
        QListWidgetItem* item =new QListWidgetItem(QString::fromStdString(playlist.getName()));

        item->setData(Qt::UserRole,playlist.getId());

        ui->playlistsListWidget->addItem(item);
    }

    refreshSongs();
}

void ListenerDashboard::refreshSongs()
{
   vector<Song> songs =getAllCatalogSongs();

    songs =MusicQueryService::searchSongsByName(songs,ui->searchLineEdit->text().toStdString());

    const QString genre =ui->genreComboBox->currentText();

    if (genre != "All Genres")
    {
        songs =MusicQueryService::filterSongsByGenre(songs,genre.toStdString());
    }

    const QString year =ui->yearComboBox->currentText();

    if (year != "All Years")
    {
        songs =MusicQueryService::filterSongsByReleaseYear(songs,year.toInt());
    }

    const QString sort =ui->sortComboBox->currentText();

    if (sort == "Name A-Z")
    {
        songs =MusicQueryService::sortSongsByName(songs,true);
    }
    else if (sort == "Name Z-A")
    {
        songs =MusicQueryService::sortSongsByName(songs,false);
    }
    else if (sort == "Year Newest-Oldest")
    {
        songs =MusicQueryService::sortSongsByReleaseYear(songs,false);
    }
    else if (sort == "Year Oldest-Newest")
    {
        songs =MusicQueryService::sortSongsByReleaseYear(songs,true);
    }

    m_displayedSongs = songs;

    ui->songsTableWidget->setRowCount(0);

    for (const Song& song : songs)
    {
        const int row =ui->songsTableWidget->rowCount();

        ui->songsTableWidget->insertRow(row);


        QTableWidgetItem* titleItem =new QTableWidgetItem(QString::fromStdString(song.getName()));

        titleItem->setData(Qt::UserRole,song.getId());

        const QString coverPath =QString::fromStdString(song.getCoverPath());

        if (!coverPath.isEmpty())
        {
            titleItem->setIcon(QIcon(coverPath));
        }

        ui->songsTableWidget->setItem(row,0,titleItem);

        QString artistName ="Unknown Artist";

        const optional<Account> artist =m_catalogService.getArtist(song.getArtistId());

        if (artist.has_value())
        {
            artistName =QString::fromStdString(artist->getFullName());
        }

        ui->songsTableWidget->setItem(row,1,new QTableWidgetItem(artistName));


        QString albumName ="Single";

        if (!song.isSingle())
        {
            const optional<Album> album =m_catalogService.getAlbum(song.getAlbumId());

            if (album.has_value())
            {
                albumName =QString::fromStdString(album->getName());
            }
        }

        ui->songsTableWidget->setItem(row,2,new QTableWidgetItem(albumName));

        ui->songsTableWidget->setItem(row,3,new QTableWidgetItem(QString::fromStdString(song.getGenre())));

        ui->songsTableWidget->setItem(row,4,new QTableWidgetItem(QString::number(song.getReleaseYear())));

        ui->songsTableWidget->setRowHeight(row,48);
    }
    updateSongButtons();
}

void ListenerDashboard::updateSongButtons()
{
    const int row =ui->songsTableWidget->currentRow();

    if (row < 0)
    {
        ui->likeButton->setEnabled(false);

        ui->addToPlaylistButton->setEnabled(false);

        ui->likeButton->setText("Like");

        return;
    }

    QTableWidgetItem* item =ui->songsTableWidget->item(row,0);


    if (item == nullptr)
    {
        ui->likeButton->setEnabled(false);

        ui->addToPlaylistButton->setEnabled(false);

        return;
    }

    ui->likeButton->setEnabled(true);

    ui->addToPlaylistButton->setEnabled(true);

    const int songId =item->data(Qt::UserRole).toInt();
    const bool liked =m_listenerService.isLiked(m_listenerId,songId);

    if (liked)
    {
        ui->likeButton->setText("Unlike");
    }
    else
    {
        ui->likeButton->setText("Like");
    }
}

void ListenerDashboard::on_likeButton_clicked()
{
    const int row =ui->songsTableWidget->currentRow();

    if (row < 0)
    {
        return;
    }

    QTableWidgetItem* item =ui->songsTableWidget->item(row,0);

    if (item == nullptr)
    {
        return;
    }

    const int songId =item->data(Qt::UserRole).toInt();

    const bool currentlyLiked =m_listenerService.isLiked(m_listenerId,songId);

    ListenerActionResult result;

    if (currentlyLiked)
    {
        result =m_listenerService.unlikeSong(m_listenerId,songId);
    }
    else
    {
        result =m_listenerService.likeSong(m_listenerId,songId);
    }


    if (!result.success)
    {
        QMessageBox::warning(this,"Error",QString::fromStdString(result.message));

        return;
    }

    updateSongButtons();
}

void ListenerDashboard::on_addToPlaylistButton_clicked()
{
    const int row =ui->songsTableWidget->currentRow();

    if (row < 0)
    {
        QMessageBox::warning(this,"No Song Selected","Please select a song first.");

        return;
    }

    QTableWidgetItem* item =ui->songsTableWidget->item(row,0);

    if (item == nullptr)
    {
        return;
    }

    const int songId =item->data(Qt::UserRole).toInt();

    ChoosePlaylistDialog dialog(m_listenerId,songId,m_listenerService,this);

    dialog.exec();
}

void ListenerDashboard::on_songsTableWidget_cellDoubleClicked(int row,int column)
{
    Q_UNUSED(column);

    QTableWidgetItem* item =ui->songsTableWidget->item(row,0);

    if (item == nullptr)
    {
        return;
    }

    const int songId =item->data(Qt::UserRole).toInt();

    MusicPlayerDialog dialog(m_displayedSongs,songId,this);

    dialog.exec();
}

void ListenerDashboard::on_artistsListWidget_itemDoubleClicked(QListWidgetItem* item)
{
    if (item == nullptr)
    {
        return;
    }

    const int artistId =item->data(Qt::UserRole).toInt();

    ArtistBrowseDialog dialog(m_listenerId,artistId,m_catalogService,m_listenerService,this);

    dialog.exec();
}

void ListenerDashboard::on_createPlaylistButton_clicked()
{
    CreatePlaylistDialog dialog(m_listenerId,m_listenerService,this);

    if (dialog.exec() == QDialog::Accepted)
    {
        refreshDashboard();
    }
}

void ListenerDashboard::on_playlistsListWidget_itemDoubleClicked(QListWidgetItem* item)
{
    if (item == nullptr)
    {
        return;
    }

    const int playlistId =item->data(Qt::UserRole).toInt();

    PlaylistSongsDialog dialog(m_listenerId,playlistId,item->text(),m_listenerService,this);

    dialog.exec();

    refreshDashboard();
}

void ListenerDashboard::on_accountButton_clicked()
{
    AccountSettingsDialog dialog(m_listenerId,m_accountService,this);

    dialog.exec();

    if (dialog.wasAccountDeleted())
    {
        accept();

        return;
    }

    refreshDashboard();
}

void ListenerDashboard::on_logoutButton_clicked()
{
    accept();
}