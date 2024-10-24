#include "loginwidget.h"
#include "ui_loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWidget::on_loginButton_clicked);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_loginButton_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();

    if (username == "admin" && password == "1234") {
        QMessageBox::information(this, "登录成功", "欢迎！");
        emit loginSuccessful();  // 触发登录成功信号
        // this->close();  // 关闭窗口
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}

