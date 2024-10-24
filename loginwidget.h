#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QMainWindow>
#include "QMessageBox"
namespace Ui {
class LoginWidget;
}

class LoginWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
signals:
    void loginSuccessful(); // 定义一个信号，用于表示登录成功
private slots:
    void on_loginButton_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
