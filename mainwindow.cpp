#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "authservice.h"
#include <QMessageBox>

MainWindow::MainWindow(AuthService& authService,QWidget *parent)
    : QMainWindow(parent),
     ui(new Ui::MainWindow),
    m_authService(authService)
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

    QMessageBox::information(this,"Login Successful","Welcome "+ QString::fromStdString(result.account->getFullName()));
}


void MainWindow::on_passwordLineEdit_returnPressed()
{
    on_loginButton_clicked();
}

