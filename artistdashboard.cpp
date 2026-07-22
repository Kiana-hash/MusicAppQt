#include "artistdashboard.h"
#include "ui_artistdashboard.h"

#include "artistservice.h"
#include "catalogservice.h"
#include "accountservice.h"
#include <QListWidgetItem>
#include "createalbumdialog.h"
#include "albumsongsdialog.h"
#include "createsongdialog.h"

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

    refreshDashboard();
}

void ArtistDashboard::refreshDashboard()
{

    optional<Account> account =m_accountService.getAccount(m_artistId);

    if (account.has_value())
    {
        ui->welcomeLabel->setText("Welcome, "+ QString::fromStdString(account->getFullName()));
    }

    ui->albumsListWidget->clear();

    vector<Album> albums =m_artistService.getAlbums(m_artistId);

    for (const Album& album : albums)
    {
        QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(album.getName()));
        item->setData(Qt::UserRole,album.getId());
        ui->albumsListWidget->addItem(item);
    }

    vector<Song> singles =m_artistService.getSingles(m_artistId);

    if (!singles.empty())
    {
        QListWidgetItem* singlesItem = new QListWidgetItem("Singles");

        singlesItem->setData(Qt::UserRole,0);

        ui->albumsListWidget->addItem(singlesItem);
    }
}

ArtistDashboard::~ArtistDashboard()
{
    delete ui;
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

    AlbumSongsDialog dialog(
        m_artistId,
        albumId,
        albumName,
        m_artistService,
        m_catalogService,
        this
        );

    dialog.exec();

    refreshDashboard();
}

void ArtistDashboard::on_createAlbumButton_clicked()
{
    CreateAlbumDialog dialog(m_artistId,m_artistService,this);

    if (dialog.exec() == QDialog::Accepted)
    {
        refreshDashboard();
    }
}

void ArtistDashboard::on_createSingleButton_clicked()
{
    CreateSongDialog dialog(m_artistId,0,m_artistService,this);

    if (dialog.exec() == QDialog::Accepted)
    {
        refreshDashboard();
    }
}


void ArtistDashboard::on_accountButton_clicked()
{

}

