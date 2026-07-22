#include "createplaylistdialog.h"
#include "ui_createplaylistdialog.h"

#include "listenerservice.h"
#include <QMessageBox>


CreatePlaylistDialog::CreatePlaylistDialog(int listenerId,ListenerService& listenerService,QWidget* parent)
    : QDialog(parent),
    ui(new Ui::CreatePlaylistDialog),
    m_listenerId(listenerId),
    m_listenerService(listenerService)
{
    ui->setupUi(this);
}


CreatePlaylistDialog::~CreatePlaylistDialog()
{
    delete ui;
}
void CreatePlaylistDialog::on_createButton_clicked()
{
    const string name =ui->playlistNameLineEdit->text().toStdString();

    const ListenerActionResult result =m_listenerService.createPlaylist(m_listenerId,name);

    if (!result.success)
    {
        QMessageBox::warning(this,"Create Playlist Failed",QString::fromStdString(result.message));

        return;
    }

    QMessageBox::information(this,"Playlist Created","The playlist was created successfully.");

    accept();
}


void CreatePlaylistDialog::on_cancelButton_clicked()
{
     reject();
}

