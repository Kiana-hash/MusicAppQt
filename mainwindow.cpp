#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "authservice.h"
#include <QMessageBox>
#include "registerdialog.h"
#include "artistdashboard.h"
#include "listenerdashboard.h"

MainWindow::MainWindow(
    AuthService& authService,
    ArtistService& artistService,
    ListenerService& listenerService,
    CatalogService& catalogService,
    AccountService& accountService,
    QWidget* parent
    )
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_authService(authService),
    m_artistService(artistService),
    m_listenerService(listenerService),
    m_catalogService(catalogService),
    m_accountService(accountService)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    string username =ui->usernameLineEdit->text().toStdString();

    string password =ui->passwordLineEdit->text().toStdString();

    AuthResult result = m_authService.login(username,password);

    if (!result.success)
    {
        QMessageBox::warning(this,"Login Failed",QString::fromStdString(result.message));

        return;
    }

    if (result.account->isArtist())
    {
        ArtistDashboard dashboard(
            result.account->getId(),
            m_artistService,
            m_catalogService,
            m_accountService,
            this
            );

        hide();

        dashboard.exec();

        show();

        return;
    }

    if (result.account->isListener())
    {
        ListenerDashboard dashboard(
            result.account->getId(),
            m_listenerService,
            m_catalogService,
            m_accountService,
            this
            );

        hide();

        dashboard.exec();

        show();

        return;
    }
}



void MainWindow::on_passwordLineEdit_returnPressed()
{
    on_loginButton_clicked();
}


void MainWindow::on_registerButton_clicked()
{
    RegisterDialog dialog(m_authService,this);
    dialog.exec();
}

