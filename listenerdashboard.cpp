#include "listenerdashboard.h"
#include "ui_listenerdashboard.h"

#include "listenerservice.h"
#include "catalogservice.h"
#include "accountservice.h"
#include <QMessageBox>
#include "artistbrowsedialog.h"


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
    refreshDashboard();
}


ListenerDashboard::~ListenerDashboard()
{
    delete ui;
}

void ListenerDashboard::refreshDashboard()
{

    optional<Account> account =m_accountService.getAccount(m_listenerId);

    if (account.has_value())
    {
        ui->welcomeLabel->setText("Welcome, "+ QString::fromStdString(account->getFullName()));
    }

    ui->artistsListWidget->clear();

    vector<Account> artists =m_catalogService.getAllArtists();


    for (const Account& artist : artists)
    {
        QListWidgetItem* item =new QListWidgetItem(QString::fromStdString(artist.getFullName()));

        item->setData(Qt::UserRole,artist.getId());

        ui->artistsListWidget->addItem(item);
    }

    ui->playlistsListWidget->clear();

    vector<Playlist> playlists = m_listenerService.getPlaylists(m_listenerId);

    for (const Playlist& playlist : playlists)
    {
        QListWidgetItem* item =new QListWidgetItem(QString::fromStdString(playlist.getName()));

        item->setData(Qt::UserRole,playlist.getId());

        ui->playlistsListWidget->addItem(item);
    }
}
void ListenerDashboard::on_logoutButton_clicked()
{
    accept();
}


void ListenerDashboard::on_artistsListWidget_itemDoubleClicked(QListWidgetItem *item)
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

}

