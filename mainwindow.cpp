#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),runTimer(new QTimer(this))
{
    ui->setupUi(this);
    // 连接QListWidget的itemClicked信号到自定义槽函数
      connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::on_listWidget_itemClicked);



  // 记录开始时间
    // 启动经过时间计时器
    elapsedTimer.start(); // 开始计时

    // 连接计时器的timeout信号到槽函数
    connect(runTimer, &QTimer::timeout, this, &MainWindow::updateRunTime);

    // 启动计时器，例如每秒更新一次
    runTimer->start(1000);
    // 立即调用一次updateRunTime以显示初始时间（可选）
    updateRunTime();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete runTimer; // 删除计时器对象
}


//打开文件
void MainWindow::on_openFile_triggered()
{
    QFileDialog dialog(nullptr, "请选择Dicom文件", QDir::homePath(), "DCM Files (*.dcm)");
    // dialog.setDirectory(QDir::homePath()); // 设置初始路径为用户的主目录
    // 也可以设置其他路径，比如：dialog.setDirectory("/path/to/your/directory");

    if (dialog.exec() != QDialog::Accepted)
    {
        // 用户取消了文件选择
        return;
    }
    QString filepath = dialog.selectedFiles().first(); // 获取用户选择的第一个文件路径

    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) // 根据文件类型调整读取模式
    {
        QMessageBox::warning(nullptr, "错误", "无法打开文件", QMessageBox::Ok);
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close(); // 虽然QTextStream的析构函数会关闭文件，但显式关闭也是一个好习惯

    // 输出文件内容到控制台（调试用）
    qDebug() << content;
}


//打开文件夹，多选文件

void MainWindow::on_openFolder_triggered()
{
    QFileDialog dialog(nullptr, "请选择Dicom文件", QDir::homePath(), "DCM Files (*.dcm)");
    dialog.setFileMode(QFileDialog::ExistingFiles); // 允许选择多个文件
    dialog.setNameFilter("DCM Files (*.dcm)"); // 设置过滤器以仅显示.dcm文件
    //判断是否取消了界面
    if (dialog.exec() != QDialog::Accepted)
    {
        // 用户取消了文件选择
        return;
    }
    QStringList filepaths = dialog.selectedFiles(); // 获取用户选择的文件路径列表
    // 清空QListWidget中的现有项（如果有的话）
    ui->listWidget->clear();
    // 遍历文件路径列表并输出文件名
    for (const QString &filepath : filepaths)
    {
        // 使用QFileInfo来获取文件名，并将其添加到QListWidget中
        QListWidgetItem *item = new QListWidgetItem(QFileInfo(filepath).fileName(), ui->listWidget);
        //输出文件名
        QFile file(filepath);
        // 注意：对于.dcm文件，通常不应该以文本模式打开，因为它们通常是二进制文件。
        // 但这里我们只是为了演示如何获取文件名，所以不会尝试读取文件内容。

        // 输出文件名到控制台
        qDebug() << QFileInfo(filepath).fileName();

        // 如果您确实需要读取.dcm文件的内容，您应该使用适当的方法来处理二进制数据。
        // 例如，您可以使用QDataStream或QFile::read()来读取二进制数据。
    }

    // 注意：由于我们没有打开文件来读取内容，所以不需要关闭它们。
    // 如果确实打开了文件，请确保在适当的时候关闭它们，无论是显式地还是通过RAII原则。
}

//退出系统
void MainWindow::on_clickExit_triggered()
{
    this->close();
}

//获取时间显示
void MainWindow::updateRunTime()
{
    // 获取自elapsedTimer启动以来的毫秒数
    qint64 elapsedMilliseconds = elapsedTimer.elapsed();

    // 计算小时、分钟和秒
    int hours = elapsedMilliseconds / 3600000;
    int minutes = (elapsedMilliseconds % 3600000) / 60000;
    int seconds = (elapsedMilliseconds % 60000) / 1000;

    // 格式化时间并显示在QLabel上
    QString runTimeText = QString("%1:%2:%3").arg(hours, 2, 10, QChar('0'))
                              .arg(minutes, 2, 10, QChar('0'))
                              .arg(seconds, 2, 10, QChar('0'));
    ui->runtime->setText(runTimeText); // 假设您的QLabel对象名称为labelRunTime
}

//触发信号
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    //计数器，点击一次，加一次
    clickNum = clickNum+1;
    if(clickNum%2!=0){
        qDebug() << item->text();
        QMessageBox::information(this, "Item Clicked", "You clicked on: " + item->text());
    }
}




