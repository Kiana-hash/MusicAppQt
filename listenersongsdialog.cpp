#include "listenersongsdialog.h"
#include "ui_listenersongsdialog.h"

#include "listenerservice.h"
#include "catalogservice.h"
#include "musicqueryservice.h"
#include "chooseplaylistdialog.h"
#include "musicplayerdialog.h"

#include <QComboBox>
#include <QLineEdit>
#include <QListWidgetItem>
#include <set>
#include <QMessageBox>

ListenerSongsDialog::ListenerSongsDialog(int listenerId,int artistId,int albumId,
    const QString& title,
    ListenerService& listenerService,
    CatalogService& catalogService,
    QWidget* parent
    )
    : QDialog(parent),
    ui(new Ui::ListenerSongsDialog),
    m_listenerId(listenerId),
    m_artistId(artistId),
    m_albumId(albumId),
    m_title(title),
    m_listenerService(listenerService),
    m_catalogService(catalogService)
{
    ui->setupUi(this);
    ui->songsListWidget->setIconSize(QSize(40,40));

    setupFilterOptions();

    connect( ui->searchLineEdit,&QLineEdit::textChanged,this,&ListenerSongsDialog::refreshSongs);

    connect(ui->genreComboBox,&QComboBox::currentTextChanged, this,&ListenerSongsDialog::refreshSongs);

    connect(ui->yearComboBox,&QComboBox::currentTextChanged,this,&ListenerSongsDialog::refreshSongs);

    connect(ui->sortComboBox,&QComboBox::currentTextChanged,this,&ListenerSongsDialog::refreshSongs);

    connect(ui->songsListWidget,&QListWidget::currentItemChanged,this,
        [this]()
        {
            updateLikeButton();
        }
        );

    refreshSongs();
}


ListenerSongsDialog::~ListenerSongsDialog()
{
    delete ui;
}

vector<Song>  ListenerSongsDialog::getBaseSongs() const
{
    if (m_albumId == 0)
    {
        return m_catalogService.getArtistSingles(m_artistId);
    }

    return m_catalogService.getAlbumSongs(m_albumId);
}

void ListenerSongsDialog::setupFilterOptions()
{
    const vector<Song> songs =getBaseSongs();

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

void ListenerSongsDialog::refreshSongs()
{
    ui->titleLabel->setText(m_title);

    vector<Song> songs =getBaseSongs();

    const string searchText =ui->searchLineEdit->text().toStdString();

    songs =MusicQueryService::searchSongsByName(songs,searchText);

    const QString selectedGenre =ui->genreComboBox->currentText();

    if (selectedGenre != "All Genres")
    {
        songs =MusicQueryService::filterSongsByGenre(songs,selectedGenre.toStdString());
    }

    const QString selectedYear =ui->yearComboBox->currentText();

    if (selectedYear != "All Years")
    {
        songs =MusicQueryService::filterSongsByReleaseYear(songs,selectedYear.toInt());
    }

    const QString selectedSort =ui->sortComboBox->currentText();

    if (selectedSort == "Name A-Z")
    {
        songs =MusicQueryService::sortSongsByName(songs,true);
    }
    else if (selectedSort == "Name Z-A")
    {
        songs =MusicQueryService::sortSongsByName(songs,false);
    }
    else if (selectedSort== "Year Oldest-Newest")
    {
        songs =MusicQueryService::sortSongsByReleaseYear(songs,true);
    }
    else if (selectedSort== "Year Newest-Oldest")
    {
        songs =MusicQueryService::sortSongsByReleaseYear(songs,false);
    }

    m_displayedSongs = songs;

    ui->songsListWidget->clear();

    for (const Song& song : songs)
    {
        QListWidgetItem* item =new QListWidgetItem(QIcon(QString::fromStdString(song.getCoverPath())),
                                                    QString::fromStdString(song.getName()));

        item->setData(Qt::UserRole,song.getId());

        ui->songsListWidget->addItem(item);
    }
    updateLikeButton();
}

void ListenerSongsDialog::updateLikeButton()
{
    QListWidgetItem* item =ui->songsListWidget->currentItem();


    if (item == nullptr)
    {
        ui->likeButton->setText("Like");

        ui->likeButton->setEnabled(false);
        ui->addToPlaylistButton->setEnabled(false);

        return;
    }


    ui->likeButton->setEnabled(true);
     ui->addToPlaylistButton->setEnabled(true);

    const int songId =item->data(Qt::UserRole).toInt();

    const bool liked =m_listenerService.isLiked(m_listenerId,songId);

    if (liked)
    {
        ui->likeButton->setText("Unlike");
    }
    else
    {
        ui->likeButton->setText("Like");
    }
}

void ListenerSongsDialog::on_likeButton_clicked()
{
    QListWidgetItem* item =ui->songsListWidget->currentItem();

    if (item == nullptr)
    {
        return;
    }

    const int songId =item->data(Qt::UserRole).toInt();

    const bool currentlyLiked =m_listenerService.isLiked(m_listenerId,songId);

    ListenerActionResult result;

    if (currentlyLiked)
    {
        result =m_listenerService.unlikeSong(m_listenerId,songId);
    }
    else
    {
        result =m_listenerService.likeSong(m_listenerId,songId);
    }


    if (!result.success)
    {
        QMessageBox::warning(this,"Error",QString::fromStdString(result.message));

        return;
    }

    updateLikeButton();
}

void ListenerSongsDialog::on_closeButton_clicked()
{
    accept();
}


void ListenerSongsDialog::on_addToPlaylistButton_clicked()
{
    QListWidgetItem* item =ui->songsListWidget->currentItem();

    if (item == nullptr)
    {
        QMessageBox::warning(this,"No Song Selected","Please select a song first.");
        return;
    }


    const int songId =item->data(Qt::UserRole).toInt();

    ChoosePlaylistDialog dialog(m_listenerId,songId,m_listenerService,this);


    if (dialog.exec() == QDialog::Accepted)
    {
        updateLikeButton();
    }
}


void ListenerSongsDialog::on_songsListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item == nullptr)
    {
        return;
    }

    const int songId =item->data(Qt::UserRole).toInt();

    MusicPlayerDialog dialog(m_displayedSongs,songId,this);

    dialog.exec();
}

