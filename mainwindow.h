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


private:
    Ui::MainWindow *ui;
    QTimer *runTimer;
    QElapsedTimer elapsedTimer; // 使用QElapsedTimer而不是QTime
    int clickNum = 0;  // 用于标记按键状态

};



#endif // MAINWINDOW_H
