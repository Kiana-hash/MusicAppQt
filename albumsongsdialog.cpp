#include "albumsongsdialog.h"
#include "ui_albumsongsdialog.h"

#include "artistservice.h"
#include "catalogservice.h"
#include "createsongdialog.h"
#include "editsongdialog.h"
#include <QMessageBox>
#include "editalbumdialog.h"
#include <set>
#include "musicqueryservice.h"

AlbumSongsDialog::AlbumSongsDialog(int artistId,int albumId,
    const QString& albumName,
    ArtistService& artistService,
    CatalogService& catalogService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::AlbumSongsDialog),
    m_artistId(artistId),
    m_albumId(albumId),
    m_albumName(albumName),
    m_artistService(artistService),
    m_catalogService(catalogService)
{
    ui->setupUi(this);
    setupFilterOptions();

    connect(ui->searchLineEdit,&QLineEdit::textChanged,this,&AlbumSongsDialog::refreshSongs);
    connect(ui->genreComboBox,&QComboBox::currentTextChanged,this,&AlbumSongsDialog::refreshSongs);
    connect(ui->yearComboBox,&QComboBox::currentTextChanged,this,&AlbumSongsDialog::refreshSongs);
    connect(ui->sortComboBox,&QComboBox::currentTextChanged,this,&AlbumSongsDialog::refreshSongs);
    refreshSongs();
}

void AlbumSongsDialog::refreshSongs()
{
    ui->titleLabel->setText(m_albumName);

    vector<Song> songs =getBaseSongs();

    const string searchText =ui->searchLineEdit->text().toStdString();

    songs =MusicQueryService::searchSongsByName(songs,searchText);

    const QString selectedGenre = ui->genreComboBox->currentText();

    if (selectedGenre != "All Genres")
    {
        songs =MusicQueryService::filterSongsByGenre(songs,selectedGenre.toStdString());
    }

    const QString selectedYear =ui->yearComboBox->currentText();

    if (selectedYear != "All Years")
    {
        const int year =selectedYear.toInt();
        songs =MusicQueryService::filterSongsByReleaseYear(songs,year);
    }

    const QString selectedSort =ui->sortComboBox->currentText();

    if (selectedSort == "Name A-Z")
    {
        songs =MusicQueryService::sortSongsByName(songs,true);
    }
    else if (selectedSort == "Name Z-A")
    {
        songs =MusicQueryService::sortSongsByName(songs,false);
    }
    else if (selectedSort == "Year Oldest-Newest" )
    {
        songs =MusicQueryService::sortSongsByReleaseYear(songs,true);
    }
    else if (selectedSort== "Year Newest-Oldest")
    {
        songs =MusicQueryService::sortSongsByReleaseYear(songs,false);
    }

    ui->songsListWidget->clear();

    for (const Song& song : songs)
    {
        QString displayText =QString::fromStdString(song.getName());

        QListWidgetItem* item =new QListWidgetItem(displayText);

        item->setData(Qt::UserRole,song.getId());

        ui->songsListWidget->addItem(item);
    }

    if (m_albumId == 0)
    {
        ui->editAlbumButton->hide();
        ui->deleteAlbumButton->hide();
    }
    else
    {
        ui->editAlbumButton->show();
        ui->deleteAlbumButton->show();
    }
}

AlbumSongsDialog::~AlbumSongsDialog()
{
    delete ui;
}
void AlbumSongsDialog::on_closeButton_clicked()
{
    accept();
}


void AlbumSongsDialog::on_addSongButton_clicked()
{
    CreateSongDialog dialog(m_artistId,m_albumId,m_artistService,this);

    if (dialog.exec() == QDialog::Accepted)
    {
        setupFilterOptions();
        refreshSongs();
    }
}


void AlbumSongsDialog::on_editSongButton_clicked()
{
    QListWidgetItem* selectedItem =ui->songsListWidget->currentItem();


    if (selectedItem == nullptr)
    {
        QMessageBox::warning(this,"No Song Selected","Please select a song first.");

        return;
    }

    const int songId =selectedItem->data(Qt::UserRole).toInt();

    EditSongDialog dialog(
        m_artistId,
        songId,
        m_artistService,
        m_catalogService,
        this
        );

    if (dialog.exec() == QDialog::Accepted)
    {
        setupFilterOptions();
        refreshSongs();
    }
}


void AlbumSongsDialog::on_deleteSongButton_clicked()
{
    QListWidgetItem* selectedItem =ui->songsListWidget->currentItem();


    if (selectedItem == nullptr)
    {
        QMessageBox::warning(this,"No Song Selected","Please select a song first.");

        return;
    }


    const QMessageBox::StandardButton answer =QMessageBox::question(this,"Delete Song",
            "Are you sure you want to delete this song?",
            QMessageBox::Yes | QMessageBox::No
            );


    if (answer != QMessageBox::Yes)
    {
        return;
    }

    const int songId =selectedItem->data(Qt::UserRole).toInt();

    bool success =m_artistService.deleteSong(m_artistId,songId);


    if (!success)
    {
        QMessageBox::warning(this,"Delete Failed","The song could not be deleted.");

        return;
    }

    setupFilterOptions();
    refreshSongs();
}

void AlbumSongsDialog::on_editAlbumButton_clicked()
{
    if (m_albumId == 0)
    {
        return;
    }

    EditAlbumDialog dialog(m_artistId,m_albumId,m_artistService,m_catalogService,this);

    if (dialog.exec() == QDialog::Accepted)
    {
        optional<Album> album =m_catalogService.getAlbum(m_albumId);

        if (album.has_value())
        {
            m_albumName =QString::fromStdString(album->getName());
        }

        refreshSongs();
    }
}

void AlbumSongsDialog::on_deleteAlbumButton_clicked()
{
    if (m_albumId == 0)
    {
        return;
    }


    QMessageBox::StandardButton answer =QMessageBox::question(this,"Delete Album",
            "Deleting this album will also delete all songs inside it.\n\n"
            "Are you sure?",
            QMessageBox::Yes |QMessageBox::No);


    if (answer != QMessageBox::Yes)
    {
        return;
    }

    bool success =m_artistService.deleteAlbum(m_artistId,m_albumId);

    if (!success)
    {
        QMessageBox::warning(this,"Delete Failed","The album could not be deleted.");

        return;
    }

    QMessageBox::information(this,"Album Deleted","The album was deleted successfully.");

    accept();
}

vector<Song>  AlbumSongsDialog::getBaseSongs() const
{
    if (m_albumId == 0)
    {
        return m_artistService.getSingles(m_artistId);
    }

    return m_catalogService.getAlbumSongs(m_albumId);
}

void AlbumSongsDialog::setupFilterOptions()
{
    vector<Song> songs =getBaseSongs();

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
}
