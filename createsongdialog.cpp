#include "createsongdialog.h"
#include "ui_createsongdialog.h"

#include "artistservice.h"
#include <QFileDialog>
#include <QMessageBox>


CreateSongDialog::CreateSongDialog(int artistId,int albumId,
    ArtistService& artistService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::CreateSongDialog),
    m_artistId(artistId),
    m_albumId(albumId),
    m_artistService(artistService)
{
    ui->setupUi(this);
}


CreateSongDialog::~CreateSongDialog()
{
    delete ui;
}

void CreateSongDialog::on_chooseAudioButton_clicked()
{
    QString filePath =QFileDialog::getOpenFileName(this,"Choose Audio File","","Audio Files (*.mp3 *.wav)");

    if (filePath.isEmpty())
    {
        return;
    }

    ui->audioPathLineEdit->setText(filePath);
}


void CreateSongDialog::on_chooseCoverButton_clicked()
{
    QString filePath =QFileDialog::getOpenFileName(this,"Choose Song Cover","","Images (*.png *.jpg *.jpeg)");

    if (filePath.isEmpty())
    {
        return;
    }

    ui->coverPathLineEdit->setText(filePath);
}


void CreateSongDialog::on_createButton_clicked()
{
    const string name =ui->songNameLineEdit->text().toStdString();

    const int releaseYear =ui->releaseYearSpinBox->value();

    const string genre =ui->genreLineEdit->text().toStdString();

    const string audioPath =ui->audioPathLineEdit->text().toStdString();

    const string coverPath =ui->coverPathLineEdit->text().toStdString();

    ArtistActionResult result;

    if (m_albumId == 0)
    {
        result =m_artistService.createSingle(m_artistId,
                name,
                releaseYear,
                genre,
                audioPath,
                coverPath
                );
    }
    else
    {
        result =m_artistService.createSong(m_artistId,
                m_albumId,
                name,
                releaseYear,
                genre,
                audioPath,
                coverPath
                );
    }


    if (!result.success)
    {
        QMessageBox::warning(this,"Create Song Failed",QString::fromStdString(result.message));
        return;
    }


    QMessageBox::information(this,"Song Created","The song was created successfully.");


    accept();
}


void CreateSongDialog::on_cancelButton_clicked()
{
    reject();
}

