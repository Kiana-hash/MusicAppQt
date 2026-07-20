#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "authservice.h"
#include <QMessageBox>
#include <QFileDialog>

RegisterDialog::RegisterDialog(AuthService& authService,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog),
      m_authService(authService)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_createAccountButton_clicked()
{
    string fullName =ui->fullNameLineEdit->text().toStdString();

    string username =ui->usernameLineEdit->text().toStdString();

    string password =ui->passwordLineEdit->text().toStdString();

    string biography =ui->biographyTextEdit->toPlainText().toStdString();

    string profilePhotoPath =ui->profilePhotoLineEdit->text().toStdString();

    string role;

    if (ui->artistRadioButton->isChecked())
    {
        role = "artist";
    }
    else
    {
        role = "listener";
    }


    AuthResult result =m_authService.registerAccount(fullName,username,biography,role,password,profilePhotoPath);

    if (!result.success)
    {
        QMessageBox::warning(this,"Registration Failed",QString::fromStdString(result.message));
        return;
    }

    QMessageBox::information(this,"Registration Successful","Your account was created successfully.");

    accept();
}


void RegisterDialog::on_cancelButton_clicked()
{
    reject();
}


void RegisterDialog::on_choosePhotoButton_clicked()
{
    QString filePath =QFileDialog::getOpenFileName(this,"Choose Profile Photo","","Images (*.png *.jpg *.jpeg)");

    if (filePath.isEmpty())
    {
        return;
    }

    ui->profilePhotoLineEdit->setText(filePath);
}

