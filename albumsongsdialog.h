#ifndef ALBUMSONGSDIALOG_H
#define ALBUMSONGSDIALOG_H

#include <QDialog>

class ArtistService;
class CatalogService;

namespace Ui
{
class AlbumSongsDialog;
}

class AlbumSongsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AlbumSongsDialog(
        int artistId,
        int albumId,
        const QString& albumName,
        ArtistService& artistService,
        CatalogService& catalogService,
        QWidget* parent = nullptr
        );

    ~AlbumSongsDialog();

private slots:
    void on_closeButton_clicked();

private:
    void refreshSongs();

    Ui::AlbumSongsDialog* ui;

    int m_artistId;
    int m_albumId;

    QString m_albumName;

    ArtistService& m_artistService;
    CatalogService& m_catalogService;
};

#endif