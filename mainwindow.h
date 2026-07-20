#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class AuthService;
class ArtistService;
class ListenerService;
class CatalogService;
class AccountService;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(
        AuthService& authService,
        ArtistService& artistService,
        ListenerService& listenerService,
        CatalogService& catalogService,
        AccountService& accountService,
        QWidget* parent = nullptr
        );
    ~MainWindow() override;

private slots:
    void on_loginButton_clicked();

    void on_passwordLineEdit_returnPressed();

    void on_registerButton_clicked();

private:
    Ui::MainWindow *ui;
    AuthService& m_authService;
    ArtistService& m_artistService;
    ListenerService& m_listenerService;
    CatalogService& m_catalogService;
    AccountService& m_accountService;
};
#endif // MAINWINDOW_H
