#include "createalbumdialog.h"
#include "ui_createalbumdialog.h"

#include "artistservice.h"
#include <QFileDialog>
#include <QMessageBox>


CreateAlbumDialog::CreateAlbumDialog(int artistId,ArtistService& artistService,QWidget* parent)
    : QDialog(parent),
    ui(new Ui::CreateAlbumDialog),
    m_artistId(artistId),
    m_artistService(artistService)
{
    ui->setupUi(this);
}


CreateAlbumDialog::~CreateAlbumDialog()
{
    delete ui;
}
void CreateAlbumDialog::on_chooseCoverButton_clicked()
{
    QString filePath =QFileDialog::getOpenFileName(this,"Choose Album Cover","","Images (*.png *.jpg *.jpeg)");

    if (filePath.isEmpty())
    {
        return;
    }

    ui->coverPathLineEdit->setText(filePath);
}


void CreateAlbumDialog::on_createButton_clicked()
{
    const string albumName =ui->albumNameLineEdit->text().toStdString();

    const string coverPath =ui->coverPathLineEdit->text().toStdString();

    const ArtistActionResult result =m_artistService.createAlbum(m_artistId,albumName,coverPath);

    if (!result.success)
    {
        QMessageBox::warning(this,"Create Album Failed",QString::fromStdString(result.message));
        return;
    }

    QMessageBox::information(this,"Album Created","Album created successfully.");

    accept();
}


void CreateAlbumDialog::on_cancelButton_clicked()
{
    reject();
}

