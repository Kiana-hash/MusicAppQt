#include "editsongdialog.h"
#include "ui_editsongdialog.h"

#include "artistservice.h"
#include "catalogservice.h"
#include <QFileDialog>
#include <QMessageBox>

EditSongDialog::EditSongDialog(int artistId,int songId,
    ArtistService& artistService,
    CatalogService& catalogService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::EditSongDialog),
    m_artistId(artistId),
    m_songId(songId),
    m_artistService(artistService),
    m_catalogService(catalogService)
{
    ui->setupUi(this);

    loadSong();
}


EditSongDialog::~EditSongDialog()
{
    delete ui;
}

void EditSongDialog::loadSong()
{
    optional<Song> song =m_catalogService.getSong(m_songId);

    if (!song.has_value())
    {
        return;
    }

    ui->songNameLineEdit->setText(QString::fromStdString(song->getName()));

    ui->releaseYearSpinBox->setValue(song->getReleaseYear());

    ui->genreLineEdit->setText(QString::fromStdString(song->getGenre()));

    ui->audioPathLineEdit->setText(QString::fromStdString(song->getAudioFilePath()));

    ui->coverPathLineEdit->setText(QString::fromStdString(song->getCoverPath()));
}

void EditSongDialog::on_chooseAudioButton_clicked()
{
    QString filePath =QFileDialog::getOpenFileName(this,"Choose Audio File","","Audio Files (*.mp3 *.wav)");

    if (filePath.isEmpty())
    {
        return;
    }

    ui->audioPathLineEdit->setText(filePath);
}


void EditSongDialog::on_chooseCoverButton_clicked()
{
    QString filePath =QFileDialog::getOpenFileName(this,"Choose Song Cover","","Images (*.png *.jpg *.jpeg)");

    if (filePath.isEmpty())
    {
        return;
    }

    ui->coverPathLineEdit->setText(filePath);
}


void EditSongDialog::on_saveButton_clicked()
{
    optional<Song> currentSong =m_catalogService.getSong(m_songId);

    if (!currentSong.has_value())
    {
        QMessageBox::warning(this,"Error","The song could not be found.");

        return;
    }


    const string name =ui->songNameLineEdit->text().toStdString();

    const int releaseYear =ui->releaseYearSpinBox->value();

    const string genre =ui->genreLineEdit->text().toStdString();

    const string audioPath = ui->audioPathLineEdit->text().toStdString();

    const string coverPath =ui->coverPathLineEdit->text().toStdString();

    bool success =m_artistService.updateSong(m_artistId,m_songId,name,releaseYear,
            genre,
            currentSong->getAlbumId(),
            audioPath,
            coverPath
            );


    if (!success)
    {
        QMessageBox::warning(this,"Edit Song Failed","The song could not be updated.");

        return;
    }

    QMessageBox::information(this,"Song Updated","The song was updated successfully.");

    accept();
}

void EditSongDialog::on_cancelButton_clicked()
{
    reject();
}

