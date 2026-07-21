#include "chooseplaylistdialog.h"
#include "ui_chooseplaylistdialog.h"

#include "listenerservice.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>


ChoosePlaylistDialog::ChoosePlaylistDialog(int listenerId,int songId,
    ListenerService& listenerService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::ChoosePlaylistDialog),
    m_listenerId(listenerId),
    m_songId(songId),
    m_listenerService(listenerService)
{
    ui->setupUi(this);

    loadPlaylists();

    connect(ui->playlistsListWidget,&QListWidget::itemDoubleClicked,this,
        [this](QListWidgetItem*)
        {
            addToSelectedPlaylist();
        });
}


ChoosePlaylistDialog::~ChoosePlaylistDialog()
{
    delete ui;
}

void ChoosePlaylistDialog::loadPlaylists()
{
    ui->playlistsListWidget->clear();

    const vector<Playlist> playlists =m_listenerService.getPlaylists(m_listenerId);

    for (const Playlist& playlist : playlists)
    {
        QListWidgetItem* item =new QListWidgetItem(QString::fromStdString(playlist.getName()));

        item->setData(Qt::UserRole,playlist.getId());

        ui->playlistsListWidget->addItem(item);
    }
}

void ChoosePlaylistDialog::addToSelectedPlaylist()
{
    QListWidgetItem* item =ui->playlistsListWidget->currentItem();


    if (item == nullptr)
    {
        QMessageBox::warning(this,"No Playlist Selected","Please select a playlist first.");

        return;
    }


    const int playlistId =item->data(Qt::UserRole).toInt();

    const ListenerActionResult result =m_listenerService.addSongToPlaylist(m_listenerId,playlistId,m_songId);


    if (!result.success)
    {
        QMessageBox::warning(this,"Add Failed",QString::fromStdString(result.message));

        return;
    }

    QMessageBox::information(this,"Song Added","The song was added to the playlist.");

    accept();
}
void ChoosePlaylistDialog::on_addButton_clicked()
{
    addToSelectedPlaylist();
}


void ChoosePlaylistDialog::on_cancelButton_clicked()
{
    reject();
}

