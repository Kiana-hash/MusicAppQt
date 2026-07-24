#include "musicplayerdialog.h"
#include "ui_musicplayerdialog.h"
#include <QAudioOutput>
#include <QFileInfo>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QSlider>
#include <QUrl>
#include "imageutils.h"
#include <QIcon>


MusicPlayerDialog::MusicPlayerDialog(const vector<Song>& songs,int startingSongId,QWidget* parent)
    : QDialog(parent),
    ui(new Ui::MusicPlayerDialog),
    m_songs(songs),
    m_currentSongId(-1),
    m_player(new QMediaPlayer(this)),
    m_audioOutput(new QAudioOutput(this))
{
    ui->setupUi(this);


    m_player->setAudioOutput( m_audioOutput);

    ui->volumeSlider->setValue(50);

    m_audioOutput->setVolume(0.5);

    ui->positionSlider->setRange(0,0);

    ui->playlistListWidget->setIconSize(QSize(40,40));

    connect( m_player,&QMediaPlayer::positionChanged,this,&MusicPlayerDialog::updatePosition);
    connect(m_player,&QMediaPlayer::durationChanged, this,&MusicPlayerDialog::updateDuration);
    connect(m_player,&QMediaPlayer::playbackStateChanged,this,&MusicPlayerDialog::updatePlaybackState);
    connect(m_player, &QMediaPlayer::errorChanged,this,&MusicPlayerDialog::handlePlayerError);

    loadPlaylist();

    for (int i = 0;i < ui->playlistListWidget->count();++i)
    {
        QListWidgetItem* item =ui->playlistListWidget->item(i);

        const int songId =item->data(Qt::UserRole).toInt();
        if (songId == startingSongId)
        {
            ui->playlistListWidget->setCurrentRow(i);

            break;
        }
    }


    playSelectedSong();
}

void MusicPlayerDialog::updatePosition(qint64 position)
{
    ui->positionSlider->setValue(static_cast<int>(position));

    ui->currentTimeLabel->setText(formatTime(position));
}

void MusicPlayerDialog::updateDuration(qint64 duration)
{
    ui->positionSlider->setRange(0,static_cast<int>(duration));

    ui->durationLabel->setText(formatTime(duration));
}

MusicPlayerDialog::~MusicPlayerDialog()
{
    delete ui;
}
void MusicPlayerDialog::loadPlaylist()
{
    ui->playlistListWidget->clear();

    for (const Song& song : m_songs)
    {
        const QString songName =QString::fromStdString(song.getName());

        const QString coverPath =QString::fromStdString(song.getCoverPath());

        QListWidgetItem* item;

        if (!coverPath.isEmpty()&& QFileInfo::exists(coverPath))
        {
            item =new QListWidgetItem(QIcon(coverPath),songName);
        }
        else
        {
            item =new QListWidgetItem(songName);
        }

        item->setData(Qt::UserRole,song.getId());

        ui->playlistListWidget->addItem(item);
    }
}

void MusicPlayerDialog::playSong(int songId)
{
    for (const Song& song : m_songs)
    {
        if (song.getId() != songId)
        {
            continue;
        }

        const QString audioPath =QString::fromStdString(song.getAudioFilePath());

        if (audioPath.isEmpty())
        {
            QMessageBox::warning(this,"Audio File Missing","This song does not have an audio file.");

            return;
        }

        if (!QFileInfo::exists(audioPath))
        {
            QMessageBox::warning(this,"Audio File Missing","The saved audio file no longer exists.");

            return;
        }

        m_currentSongId = song.getId();

        ui->songTitleLabel->setText(QString::fromStdString(song.getName()));

        const QString coverPath =QString::fromStdString(song.getCoverPath());

        ImageUtils::displayImage(ui->coverLabel,coverPath);

        m_player->setSource(QUrl::fromLocalFile(audioPath));

        m_player->play();

        return;
    }

    QMessageBox::warning(this,"Song Error","The selected song could not be found.");
}

void MusicPlayerDialog::updatePlaybackState(QMediaPlayer::PlaybackState state)
{
    if (state== QMediaPlayer::PlayingState)
    {
        ui->statusLabel->setText("Playing");
        return;
    }

    if (state== QMediaPlayer::PausedState)
    {
        ui->statusLabel->setText("Paused");
        return;
    }

    ui->statusLabel->setText("Stopped");
}

void MusicPlayerDialog::handlePlayerError()
{
    if (m_player->error()== QMediaPlayer::NoError)
    {
        return;
    }

    QString message =m_player->errorString();

    if (message.isEmpty())
    {
        message ="The audio file could not be played.";
    }

     QMessageBox::warning(this,"Playback Error",message);
}

void MusicPlayerDialog::playSelectedSong()
{
    QListWidgetItem* item =ui->playlistListWidget->currentItem();

    if (item == nullptr)
    {
        return;
    }

    const int songId =item->data(Qt::UserRole).toInt();


    if (songId == m_currentSongId&& !m_player->source().isEmpty())
    {
        m_player->play();

        return;
    }


    playSong(songId);
}
void MusicPlayerDialog::on_playButton_clicked()
{
    playSelectedSong();
}
void MusicPlayerDialog::on_pauseButton_clicked()
{
    m_player->pause();
}

void MusicPlayerDialog::on_stopButton_clicked()
{
    m_player->stop();
}

void MusicPlayerDialog::on_nextButton_clicked()
{
    const int count =ui->playlistListWidget->count();

    if (count == 0)
    {
        return;
    }

    int currentRow =ui->playlistListWidget->currentRow();

    currentRow++;

    if (currentRow >= count)
    {
        currentRow = 0;
    }

    ui->playlistListWidget->setCurrentRow(currentRow);

     playSelectedSong();
}

void MusicPlayerDialog::on_previousButton_clicked()
{
    const int count =ui->playlistListWidget->count();

    if (count == 0)
    {
        return;
    }


    int currentRow =ui->playlistListWidget->currentRow();

        currentRow--;

    if (currentRow < 0)
    {
        currentRow = count - 1;
    }

    ui->playlistListWidget->setCurrentRow(currentRow);

    playSelectedSong();

}

void MusicPlayerDialog::on_playlistListWidget_itemDoubleClicked(QListWidgetItem* item)
{
    if (item == nullptr)
    {
         return;
    }

    const int songId =item->data(Qt::UserRole).toInt();

    playSong(songId);
}


void MusicPlayerDialog::on_closeButton_clicked()
{
    m_player->stop();

    accept();
}

QString MusicPlayerDialog::formatTime(qint64 milliseconds) const
{
    const qint64 totalSeconds =milliseconds / 1000;

    const qint64 minutes =totalSeconds / 60;

    const qint64 seconds =totalSeconds % 60;

    QString minutesText =QString::number(minutes);

    QString secondsText =QString::number(seconds);

    if (minutes < 10)
    {
        minutesText ="0" + minutesText;
    }

    if (seconds < 10)
    {
        secondsText ="0" + secondsText;
    }
    return minutesText+ ":"+ secondsText;
}

void MusicPlayerDialog::on_positionSlider_sliderMoved(int position)
{
    m_player->setPosition(position);
}

void MusicPlayerDialog::on_volumeSlider_valueChanged(int volume)
{
    m_audioOutput->setVolume(volume / 100.0);
}

