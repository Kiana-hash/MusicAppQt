#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

class AuthService;

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(AuthService& authService,QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_createAccountButton_clicked();

    void on_cancelButton_clicked();

    void on_choosePhotoButton_clicked();

private:
    Ui::RegisterDialog *ui;
    AuthService& m_authService;
};

#endif // REGISTERDIALOG_H
