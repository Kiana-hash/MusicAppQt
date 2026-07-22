#include "playlistsongsdialog.h"
#include "ui_playlistsongsdialog.h"

#include "listenerservice.h"
#include "musicqueryservice.h"

#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include<QMessageBox>
#include <set>
#include <QInputDialog>


PlaylistSongsDialog::PlaylistSongsDialog(int listenerId,int playlistId,
    const QString& playlistName,
    ListenerService& listenerService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::PlaylistSongsDialog),
    m_listenerId(listenerId),
    m_playlistId(playlistId),
    m_playlistName(playlistName),
    m_listenerService(listenerService)
{
    ui->setupUi(this);
    ui->songsListWidget->setIconSize(QSize(40,40));

    setupFilterOptions();

    connect(ui->searchLineEdit,&QLineEdit::textChanged,this,&PlaylistSongsDialog::refreshSongs);

    connect(ui->genreComboBox,&QComboBox::currentTextChanged,this,&PlaylistSongsDialog::refreshSongs);

    connect(ui->yearComboBox,&QComboBox::currentTextChanged,this,&PlaylistSongsDialog::refreshSongs);

    connect(ui->sortComboBox,&QComboBox::currentTextChanged,this,&PlaylistSongsDialog::refreshSongs);

    connect(ui->songsListWidget,&QListWidget::currentItemChanged,this,
        [this]()
        {
            updateButtons();
        });


    refreshSongs();
}


PlaylistSongsDialog::~PlaylistSongsDialog()
{
    delete ui;
}

vector<Song>  PlaylistSongsDialog::getBaseSongs() const
{
    return m_listenerService.getPlaylistSongs(m_listenerId,m_playlistId);
}

void PlaylistSongsDialog::setupFilterOptions()
{
    const vector<Song> songs = getBaseSongs();

    set<string> genres;
    set<int> years;

    for (const Song& song : songs)
    {
        genres.insert(song.getGenre());

        years.insert(song.getReleaseYear());
    }


    ui->genreComboBox->clear();

    ui->genreComboBox->addItem("All Genres");

    for (const string& genre : genres)
    {
        ui->genreComboBox->addItem(QString::fromStdString(genre));
    }

    ui->yearComboBox->clear();

    ui->yearComboBox->addItem("All Years");

    for (int year : years)
    {
        ui->yearComboBox->addItem(QString::number(year));
    }
}

void PlaylistSongsDialog::refreshSongs()
{
    ui->titleLabel->setText(m_playlistName);

    vector<Song> songs =getBaseSongs();

    songs =MusicQueryService::searchSongsByName(songs,ui->searchLineEdit->text().toStdString());

    const QString genre =ui->genreComboBox->currentText();

    if (genre != "All Genres")
    {
        songs =MusicQueryService::filterSongsByGenre(songs,genre.toStdString());
    }

    const QString year =ui->yearComboBox->currentText();

    if (year != "All Years")
    {
        songs =MusicQueryService::filterSongsByReleaseYear(songs,year.toInt());
    }

    const QString sort =ui->sortComboBox->currentText();

    if (sort == "Name A-Z")
    {
        songs =MusicQueryService::sortSongsByName(songs,true);
    }
    else if (sort == "Name Z-A")
    {
        songs =MusicQueryService::sortSongsByName(songs,false);
    }
    else if (sort == "Year Oldest-Newest")
    {
        songs =MusicQueryService::sortSongsByReleaseYear(songs,true);
    }
    else if (sort == "Year Newest-Oldest")
    {
        songs =MusicQueryService::sortSongsByReleaseYear(songs,false);
    }

    ui->songsListWidget->clear();

    for (const Song& song : songs)
    {
        QListWidgetItem* item =new QListWidgetItem(QIcon(QString::fromStdString(song.getCoverPath())),
                                                    QString::fromStdString(song.getName()));

        item->setData(Qt::UserRole,song.getId());

        ui->songsListWidget->addItem(item);
    }

    updateButtons();
}

void PlaylistSongsDialog::updateButtons()
{
    const bool songSelected =ui->songsListWidget->currentItem()!= nullptr;

    ui->removeSongButton->setEnabled(songSelected);

    const bool isFavoriteSongs =m_playlistName== "Favorite Songs";

    ui->renamePlaylistButton->setEnabled(!isFavoriteSongs);

    ui->deletePlaylistButton->setEnabled(!isFavoriteSongs);
}
void PlaylistSongsDialog::on_removeSongButton_clicked()
{
    QListWidgetItem* item =ui->songsListWidget->currentItem();

    if (item == nullptr)
    {
        return;
    }

    const int songId =item->data(Qt::UserRole).toInt();

    const QMessageBox::StandardButton answer =QMessageBox::question(this,"Remove Song",
            "Remove this song from the playlist?",
            QMessageBox::Yes |QMessageBox::No);

    if (answer != QMessageBox::Yes)
    {
        return;
    }

    const ListenerActionResult result =m_listenerService.removeSongFromPlaylist(m_listenerId,m_playlistId,songId);

    if (!result.success)
    {
        QMessageBox::warning(this,"Remove Failed",QString::fromStdString(result.message));

        return;
    }

    setupFilterOptions();

    refreshSongs();
}


void PlaylistSongsDialog::on_renamePlaylistButton_clicked()
{
    if (m_playlistName == "Favorite Songs")
    {
        return;
    }

    bool ok = false;

    const QString newName =QInputDialog::getText(this,"Rename Playlist","New playlist name:",
            QLineEdit::Normal,
            m_playlistName,
            &ok
            );


    if (!ok)
    {
        return;
    }

    const ListenerActionResult result =m_listenerService.renamePlaylist(m_listenerId,m_playlistId,newName.toStdString());

    if (!result.success)
    {
        QMessageBox::warning(this,"Rename Failed",QString::fromStdString(result.message));

        return;
    }

    m_playlistName = newName;

    refreshSongs();
}


void PlaylistSongsDialog::on_deletePlaylistButton_clicked()
{
    if (m_playlistName == "Favorite Songs")
    {
        return;
    }

    const QMessageBox::StandardButton answer =QMessageBox::question(this,"Delete Playlist",
            "Are you sure you want to delete this playlist?",
            QMessageBox::Yes |QMessageBox::No);

    if (answer != QMessageBox::Yes)
    {
        return;
    }

    const ListenerActionResult result =m_listenerService.deletePlaylist(m_listenerId,m_playlistId);

    if (!result.success)
    {
        QMessageBox::warning(this,"Delete Failed",QString::fromStdString(result.message));

        return;
    }

    accept();
}


void PlaylistSongsDialog::on_closeButton_clicked()
{
    accept();
}

