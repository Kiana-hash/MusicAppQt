#ifndef EDITSONGDIALOG_H
#define EDITSONGDIALOG_H

#include <QDialog>

class ArtistService;
class CatalogService;

namespace Ui
{
class EditSongDialog;
}

class EditSongDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditSongDialog(int artistId,int songId,
        ArtistService& artistService,
        CatalogService& catalogService,
        QWidget* parent = nullptr
        );

    ~EditSongDialog();

private slots:
    void on_chooseAudioButton_clicked();

    void on_chooseCoverButton_clicked();

    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    void loadSong();

    Ui::EditSongDialog* ui;

    int m_artistId;
    int m_songId;

    ArtistService& m_artistService;
    CatalogService& m_catalogService;
};

#endif