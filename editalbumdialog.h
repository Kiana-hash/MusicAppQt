#ifndef EDITALBUMDIALOG_H
#define EDITALBUMDIALOG_H

#include <QDialog>

class ArtistService;
class CatalogService;

namespace Ui
{
class EditAlbumDialog;
}

class EditAlbumDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditAlbumDialog(int artistId,int albumId,
        ArtistService& artistService,
        CatalogService& catalogService,
        QWidget* parent = nullptr
        );

    ~EditAlbumDialog();

private slots:


    void on_chooseCoverButton_clicked();

    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    void loadAlbum();

    Ui::EditAlbumDialog* ui;

    int m_artistId;
    int m_albumId;

    ArtistService& m_artistService;
    CatalogService& m_catalogService;
};

#endif