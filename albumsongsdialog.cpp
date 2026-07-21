#include "albumsongsdialog.h"
#include "ui_albumsongsdialog.h"

#include "artistservice.h"
#include "catalogservice.h"
#include "createsongdialog.h"
#include "editsongdialog.h"
#include <QMessageBox>
#include "editalbumdialog.h"


AlbumSongsDialog::AlbumSongsDialog(
    int artistId,
    int albumId,
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

    refreshSongs();
}

void AlbumSongsDialog::refreshSongs()
{
    ui->titleLabel->setText(m_albumName);
    ui->songsListWidget->clear();
    vector<Song> songs;

    if (m_albumId == 0)
    {
        songs =m_artistService.getSingles(m_artistId);
        ui->editAlbumButton->hide();
        ui->deleteAlbumButton->hide();
    }
    else
    {
        songs =m_catalogService.getAlbumSongs(m_albumId);

        ui->editAlbumButton->show();
        ui->deleteAlbumButton->show();
    }


    for (const Song& song : songs)
    {
        QListWidgetItem* item =new QListWidgetItem(QString::fromStdString(song.getName()));

        item->setData(Qt::UserRole,song.getId());

        ui->songsListWidget->addItem(item);
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

