#include "editalbumdialog.h"
#include "ui_editalbumdialog.h"

#include "artistservice.h"
#include "catalogservice.h"
#include <QFileDialog>
#include <QMessageBox>


EditAlbumDialog::EditAlbumDialog(int artistId,int albumId,
    ArtistService& artistService,
    CatalogService& catalogService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::EditAlbumDialog),
    m_artistId(artistId),
    m_albumId(albumId),
    m_artistService(artistService),
    m_catalogService(catalogService)
{
    ui->setupUi(this);

    loadAlbum();
}


EditAlbumDialog::~EditAlbumDialog()
{
    delete ui;
}

void EditAlbumDialog::loadAlbum()
{
    optional<Album> album =m_catalogService.getAlbum(m_albumId);

    if (!album.has_value())
    {
        return;
    }

    ui->albumNameLineEdit->setText(QString::fromStdString(album->getName()));

    ui->coverPathLineEdit->setText(QString::fromStdString(album->getCoverPath()));
}
void EditAlbumDialog::on_chooseCoverButton_clicked()
{
    QString filePath =QFileDialog::getOpenFileName(this,"Choose Album Cover","","Images (*.png *.jpg *.jpeg)");

    if (filePath.isEmpty())
    {
        return;
    }

    ui->coverPathLineEdit->setText(filePath);
}


void EditAlbumDialog::on_saveButton_clicked()
{
    const string name =ui->albumNameLineEdit->text().toStdString();

    const string coverPath =ui->coverPathLineEdit->text().toStdString();

    bool success =m_artistService.updateAlbum(m_artistId,m_albumId,name,coverPath);

    if (!success)
    {
        QMessageBox::warning(this,"Edit Album Failed","The album could not be updated.");

        return;
    }
    QMessageBox::information(this,"Album Updated","The album was updated successfully.");

    accept();
}


void EditAlbumDialog::on_cancelButton_clicked()
{
    reject();
}

