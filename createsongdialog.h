#ifndef CREATESONGDIALOG_H
#define CREATESONGDIALOG_H

#include <QDialog>

class ArtistService;

namespace Ui
{
class CreateSongDialog;
}

class CreateSongDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSongDialog(int artistId,int albumId,
        ArtistService& artistService,
        QWidget* parent = nullptr
        );

    ~CreateSongDialog();

private slots:
    void on_chooseAudioButton_clicked();

    void on_chooseCoverButton_clicked();

    void on_createButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CreateSongDialog* ui;

    int m_artistId;
    int m_albumId;

    ArtistService& m_artistService;
};

#endif