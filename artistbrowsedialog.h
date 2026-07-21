#ifndef ARTISTBROWSEDIALOG_H
#define ARTISTBROWSEDIALOG_H

#include <QDialog>
#include<QListWidgetItem>

class CatalogService;
class ListenerService;

namespace Ui
{
class ArtistBrowseDialog;
}

class ArtistBrowseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ArtistBrowseDialog(int listenerId,int artistId,
        CatalogService& catalogService,
        ListenerService& listenerService,
        QWidget* parent = nullptr
        );

    ~ArtistBrowseDialog();

private slots:
    void on_closeButton_clicked();

    void on_albumsListWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    void refreshArtist();

    Ui::ArtistBrowseDialog* ui;

    int m_listenerId;
    int m_artistId;

    CatalogService& m_catalogService;
    ListenerService& m_listenerService;
};

#endif