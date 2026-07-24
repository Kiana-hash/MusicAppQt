#ifndef LISTENERSONGSDIALOG_H
#define LISTENERSONGSDIALOG_H

#include <QDialog>
#include "song.h"
#include <vector>
#include <QListWidgetItem>

class ListenerService;
class CatalogService;

namespace Ui
{
class ListenerSongsDialog;
}

class ListenerSongsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ListenerSongsDialog(int listenerId,int artistId,int albumId, const QString& title,
        ListenerService& listenerService,
         CatalogService& catalogService,
                QWidget* parent = nullptr);

    ~ListenerSongsDialog();

private slots:
    void on_likeButton_clicked();

    void on_closeButton_clicked();

    void on_addToPlaylistButton_clicked();

    void on_songsListWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    vector<Song> getBaseSongs() const;

    void setupFilterOptions();

    void refreshSongs();

    void updateLikeButton();

    Ui::ListenerSongsDialog* ui;

    int m_listenerId;
    int m_artistId;
    int m_albumId;

    QString m_title;

    ListenerService& m_listenerService;
    CatalogService& m_catalogService;
    vector<Song> m_displayedSongs;
};

#endif