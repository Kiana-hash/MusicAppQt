#include "artistdashboard.h"
#include "ui_artistdashboard.h"
#include "artistservice.h"
#include "catalogservice.h"
#include "accountservice.h"
#include <QListWidgetItem>
#include "createalbumdialog.h"
#include "albumsongsdialog.h"
#include "createsongdialog.h"
#include "accountsettingsdialog.h"
#include "imageutils.h"
#include <QIcon>
#include "musicqueryservice.h"
#include "musicplayerdialog.h"
#include <QComboBox>
#include <QHeaderView>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <set>

ArtistDashboard::ArtistDashboard(
    int artistId,
    ArtistService& artistService,
    CatalogService& catalogService,
    AccountService& accountService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::ArtistDashboard),
    m_artistId(artistId),
    m_artistService(artistService),
    m_catalogService(catalogService),
    m_accountService(accountService)
{
    ui->setupUi(this);
    ui->albumsListWidget->setIconSize(QSize(65, 65));
    ui->songsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->songsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->songsTableWidget->verticalHeader()->setVisible(false);
    ui->songsTableWidget->setIconSize(QSize(40, 40));
    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->songsTableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);

    setupFilterOptions();

    connect(ui->searchLineEdit,&QLineEdit::textChanged,this,&ArtistDashboard::refreshSongs);

    connect(ui->genreComboBox,&QComboBox::currentTextChanged,this,&ArtistDashboard::refreshSongs);

    connect(ui->yearComboBox,&QComboBox::currentTextChanged,this,&ArtistDashboard::refreshSongs);

    connect(ui->sortComboBox,&QComboBox::currentTextChanged,this,&ArtistDashboard::refreshSongs);

    refreshDashboard();
}

void ArtistDashboard::refreshDashboard()
{
    optional<Account> account =m_accountService.getAccount(m_artistId);

    if (!account.has_value())
    {
        return;
    }

    ui->welcomeLabel->setText("Welcome, "+ QString::fromStdString(account->getFullName()));

    ImageUtils::displayImage(ui->profilePhotoLabel,QString::fromStdString(account->getProfilePhotoPath()));

    ui->albumsListWidget->clear();

    vector<Album> albums =m_artistService.getAlbums(m_artistId);

    for (const Album& album : albums)
    {
        QListWidgetItem* item =new QListWidgetItem(QIcon(QString::fromStdString(album.getCoverPath())),QString::fromStdString(album.getName()));

        item->setData(Qt::UserRole,album.getId());
        ui->albumsListWidget->addItem(item);
    }

    vector<Song> singles =m_artistService.getSingles(m_artistId);

    if (!singles.empty())
    {
        QListWidgetItem* singlesItem =new QListWidgetItem("Singles");

        singlesItem->setData(Qt::UserRole,0);
        ui->albumsListWidget->addItem(singlesItem);
    }

    ui->albumCountLabel->setText(QString::number(albums.size()));

    ui->singleCountLabel->setText(QString::number(singles.size()));

    refreshSongs();
}

ArtistDashboard::~ArtistDashboard()
{
    delete ui;
}
vector<Song>  ArtistDashboard::getArtistSongs() const
{
    vector<Song> songs;
    const vector<Song> singles =m_catalogService.getArtistSingles(m_artistId);

    for (const Song& song : singles)
    {
        songs.push_back(song);
    }

    const vector<Album> albums =m_catalogService.getArtistAlbums(m_artistId);

    for (const Album& album : albums)
    {
        const vector<Song> albumSongs =m_catalogService.getAlbumSongs(album.getId());

        for (const Song& song : albumSongs)
        {
            songs.push_back(song);
        }
    }

    return songs;
}

void ArtistDashboard::setupFilterOptions()
{
    const vector<Song> songs =getArtistSongs();

    set<string> genres;
    set<int> years;

    for (const Song& song : songs)
    {
        genres.insert(song.getGenre());

        years.insert(song.getReleaseYear());
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

void ArtistDashboard::refreshSongs()
{
    vector<Song> songs =getArtistSongs();

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

        QString releaseName ="Single";

        if (song.getAlbumId() != 0)
        {
            const optional<Album> album =m_catalogService.getAlbum(song.getAlbumId());

            if (album.has_value())
            {
                releaseName =QString::fromStdString(album->getName());
            }
        }

        ui->songsTableWidget->setItem(row,1,new QTableWidgetItem(releaseName));

        ui->songsTableWidget->setItem(row,2,new QTableWidgetItem(QString::fromStdString(song.getGenre())));

        ui->songsTableWidget->setItem(row,3,new QTableWidgetItem(QString::number(song.getReleaseYear())));

        ui->songsTableWidget->setRowHeight(row,46);
    }

    ui->songsTitleLabel->setText("YOUR SONGS  •  "+ QString::number(songs.size()));
}

void ArtistDashboard::on_logoutButton_clicked()
{
    accept();
}

void ArtistDashboard::on_albumsListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item == nullptr)
    {
        return;
    }

    const int albumId =item->data(Qt::UserRole).toInt();

    const QString albumName =item->text();

    AlbumSongsDialog dialog(m_artistId,albumId,albumName,m_artistService,m_catalogService,this);

    dialog.exec();

    refreshDashboard();
}

void ArtistDashboard::on_createAlbumButton_clicked()
{
    CreateAlbumDialog dialog(m_artistId,m_artistService,this);

    if (dialog.exec() == QDialog::Accepted)
    {
        setupFilterOptions();

        refreshDashboard();
    }
}

void ArtistDashboard::on_createSingleButton_clicked()
{
    CreateSongDialog dialog(m_artistId,0,m_artistService,this);

    if (dialog.exec() == QDialog::Accepted)
    {

        setupFilterOptions();

        refreshDashboard();
    }
}


void ArtistDashboard::on_accountButton_clicked()
{
    AccountSettingsDialog dialog(m_artistId,m_accountService,this);

    dialog.exec();

    if (dialog.wasAccountDeleted())
    {
        accept();
        return;
    }

    refreshDashboard();
}

void ArtistDashboard::on_songsTableWidget_cellDoubleClicked(int row,int column)
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

