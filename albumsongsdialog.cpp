#include "albumsongsdialog.h"
#include "ui_albumsongsdialog.h"

#include "artistservice.h"
#include "catalogservice.h"


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

