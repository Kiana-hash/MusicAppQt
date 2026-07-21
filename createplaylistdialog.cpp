#include "createplaylistdialog.h"
#include "ui_createplaylistdialog.h"

CreatePlaylistDialog::CreatePlaylistDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreatePlaylistDialog)
{
    ui->setupUi(this);
}

CreatePlaylistDialog::~CreatePlaylistDialog()
{
    delete ui;
}
