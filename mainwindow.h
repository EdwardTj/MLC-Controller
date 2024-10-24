#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QFileDialog"
#include "QMessageBox"
#include "QLabel"
#include "QTimer"
#include "QVBoxLayout"
#include <QElapsedTimer>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPainter>
#include <QPixmap>
#include <QDesktopServices>
#include <QUrl>
#include <QStringList>
#include <QString>
#include "loginwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    //打开文件
    void on_openFile_triggered();
    //打开文件夹
    void on_openFolder_triggered();
    //退出系统
    void on_clickExit_triggered();
    //获取更新时间
    void updateRunTime();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    //读取多页直准器叶片数据
    void loaddata(const QString &filePath);
    //添加Logo
    void paintLogo(const QString &pclPath);


    void on_actiongu_triggered();

    void on_actionban_triggered();

    void on_actionsss_triggered();

    void on_actionre_triggered();

    void on_action_triggered();

    void on_actionstop_triggered();

    void on_actionss_triggered();

    //处理文件名字
    void split_file_name(const QString &filename);

    //加载数据和视野
    void get_data_and_view(const QString &filepath);


private:
    Ui::MainWindow *ui;
    QTimer *runTimer;
    QElapsedTimer elapsedTimer; // 使用QElapsedTimer而不是QTime
        LoginWidget *loginWidget; // 登录窗口指针
    int clickNum = 0;  // 用于标记按键状态
    void setRunStatus(int flag);//设置设备运行状态
protected:
    void drawMLCView();
private slots:
    void showLoginDialog(); // 显示登录对话框

};



#endif // MAINWINDOW_H
