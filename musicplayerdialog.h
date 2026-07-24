#ifndef MUSICPLAYERDIALOG_H
#define MUSICPLAYERDIALOG_H

#include <QDialog>
#include <QMediaPlayer>
#include "song.h"
#include <vector>

class QAudioOutput;
class QListWidgetItem;


namespace Ui
{
class MusicPlayerDialog;
}


class MusicPlayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MusicPlayerDialog(const vector<Song>& songs,int startingSongId,QWidget* parent = nullptr);

    ~MusicPlayerDialog();

private slots:

    void on_playButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_nextButton_clicked();
    void on_previousButton_clicked();
    void on_closeButton_clicked();
    void on_positionSlider_sliderMoved(int position);
    void on_volumeSlider_valueChanged(int volume);
    void on_playlistListWidget_itemDoubleClicked(QListWidgetItem* item);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void updatePlaybackState(QMediaPlayer::PlaybackState state);
    void handlePlayerError();
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);


private:
    void loadPlaylist();
    void playSelectedSong();
    void playSong(int songId);
    QString formatTime(qint64 milliseconds) const;

    Ui::MusicPlayerDialog* ui;

    vector<Song> m_songs;

    int m_currentSongId;

    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
};

#endif