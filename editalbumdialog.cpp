#include "editalbumdialog.h"
#include "ui_editalbumdialog.h"

EditAlbumDialog::EditAlbumDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditAlbumDialog)
{
    ui->setupUi(this);
}

EditAlbumDialog::~EditAlbumDialog()
{
    delete ui;
}
