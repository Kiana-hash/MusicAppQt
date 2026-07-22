#include "artistbrowsedialog.h"
#include "ui_artistbrowsedialog.h"

#include "catalogservice.h"
#include "listenerservice.h"
#include <QListWidgetItem>
#include "listenersongsdialog.h"


ArtistBrowseDialog::ArtistBrowseDialog(int listenerId,int artistId,
    CatalogService& catalogService,
    ListenerService& listenerService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::ArtistBrowseDialog),
    m_listenerId(listenerId),
    m_artistId(artistId),
    m_catalogService(catalogService),
    m_listenerService(listenerService)
{
    ui->setupUi(this);
    ui->albumsListWidget->setIconSize(QSize(60, 60));

    refreshArtist();
}


ArtistBrowseDialog::~ArtistBrowseDialog()
{
    delete ui;
}

void ArtistBrowseDialog::refreshArtist()
{
    ui->albumsListWidget->clear();

    optional<Account> artist =m_catalogService.getArtist(m_artistId);

    if (artist.has_value())
    {
        ui->artistNameLabel->setText(QString::fromStdString(artist->getFullName()+": "));
    }

    vector<Album> albums =m_catalogService.getArtistAlbums(m_artistId);

    for (const Album& album : albums)
    {
        QListWidgetItem* item =new QListWidgetItem(QIcon(QString::fromStdString(album.getCoverPath())),
                QString::fromStdString(album.getName()));

        item->setData(Qt::UserRole,album.getId());

        ui->albumsListWidget->addItem(item);
    }

    vector<Song> singles =m_catalogService.getArtistSingles(m_artistId);

    if (!singles.empty())
    {
        QListWidgetItem* singlesItem =new QListWidgetItem("Singles");

        singlesItem->setData(Qt::UserRole,0);

        ui->albumsListWidget->addItem( singlesItem);
    }
}
void ArtistBrowseDialog::on_closeButton_clicked()
{
    accept();
}


void ArtistBrowseDialog::on_albumsListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item == nullptr)
    {
        return;
    }

    const int albumId =item->data(Qt::UserRole).toInt();

    ListenerSongsDialog dialog(m_listenerId,m_artistId,albumId,item->text(),m_listenerService,m_catalogService,this);

    dialog.exec();
}

