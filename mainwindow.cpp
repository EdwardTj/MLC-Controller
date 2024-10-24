#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),runTimer(new QTimer(this))
{
    ui->setupUi(this);
    // 在构造函数中隐藏主窗口
    this->hide();  // 隐藏窗口
    this->setVisible(false);  // 设置窗口不可见
    // QTimer::singleShot(0, this, &MainWindow::showLoginDialog);  // 立即调用 showLoginDialog()
    // showLoginDialog();  // 显示登录对话框

    // 连接QListWidget的itemClicked信号到自定义槽函数
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::on_listWidget_itemClicked);
    paintLogo("D:/zm/dcm/GomROS.png");  // 替换为实际的图片路径
    ui->picLable->setMinimumSize(100, 100); // 设置 QLabel 的最小尺寸
    ui->picLable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
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



    // 清空QListWidget中的现有项（如果有的话）
    ui->listWidget->clear();
    // 使用QFileInfo来获取文件名，并将其添加到QListWidget中
    QString fileName = QFileInfo(filepath).fileName();
    QListWidgetItem *item = new QListWidgetItem(fileName, ui->listWidget);
    // 输出文件内容到控制台（调试用）
    // qDebug() << content;
    split_file_name(fileName);
    //加载数据
    QString filePath = "D:/zm/dcm/datainof.csv"; // 替换为你的 CSV 文件路径
    get_data_and_view(filePath);

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
        // QFile file(filepath);
        // 注意：对于.dcm文件，通常不应该以文本模式打开，因为它们通常是二进制文件。
        // 但这里我们只是为了演示如何获取文件名，所以不会尝试读取文件内容。

        // 输出文件名到控制台
        // qDebug() << QFileInfo(filepath).fileName();

        // 如果您确实需要读取.dcm文件的内容，您应该使用适当的方法来处理二进制数据。
        // 例如，您可以使用QDataStream或QFile::read()来读取二进制数据。
    }


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
        QString itemText = item->text(); // 获取项的文本
        QMessageBox::StandardButton reply;
        // 显示带有确定和取消的消息框
        reply = QMessageBox::question(this, "提示", "确定要打开: " + itemText + " 吗？",
                                      QMessageBox::Yes | QMessageBox::No);

        // 根据用户选择的按钮进行相应处理
        if (reply == QMessageBox::Yes) {
            // 用户点击了“确定”
            qDebug("用户选择了打开: %s", qPrintable(itemText));
            split_file_name(item->text());
            QString filePath = "D:/zm/dcm/datainof.csv"; // 替换为你的 CSV 文件路径
            get_data_and_view(filePath);
            // 在这里添加打开网站或文件的代码
        } else {
            // 用户点击了“取消”
            qDebug("用户选择了取消");
        }
    }
}


//加载数据
void MainWindow::loaddata(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QStringList headers = in.readLine().split(',');
    ui->tableWidget->setColumnCount(headers.size());
     ui->tableWidget->setHorizontalHeaderLabels(headers);

    int row = 0;
    while (!in.atEnd()) {
        QStringList lineData = in.readLine().split(',');
         ui->tableWidget->insertRow(row);
        for (int column = 0; column < lineData.size(); ++column) {
            QTableWidgetItem *item = new QTableWidgetItem(lineData[column].trimmed());
            item->setTextAlignment(Qt::AlignCenter);  // 数据居中
             ui->tableWidget->setItem(row, column, item);
        }
        ui->tableWidget->setRowHeight(row, 20);  // 设置每行高度为30像素
        row++;
    }

    // 设置列宽自动适应
    for (int i = 0; i < headers.size(); ++i) {
         ui->tableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);  // 列宽度自动伸展
    }

    ui->tableWidget->verticalHeader()->setVisible(false);  // 隐藏行号

}


//添加logo
void MainWindow::paintLogo(const QString &pclPath){
    // QImage image(pclPath);  // 加载图片为QImage
    // // 调整图片大小，保持纵横比
    // QSize labelSize = ui->label->size();
    // QImage scaledImage = image.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // ui->picLable->setPixmap(QPixmap::fromImage(scaledImage));  // 将调整后的QImage转换为QPixmap并设置到QLabel
    // ui->picLable->setScaledContents(true);  // 使图片适应QLabel大小
    // drawMLCView();
}
//设置设备运行状态
void  MainWindow::setRunStatus(int flag){
    switch(flag){
    case 0:
        ui->statusLabel_1->setText("");
        ui->statusLabel_2->setText("");
        ui->statusLabel_3->setText("");
        ui->statusLabel_4->setText("");
        break;
    case 1:
        ui->statusLabel_1->setText("√");
        ui->statusLabel_2->setText("");
        ui->statusLabel_3->setText("");
        ui->statusLabel_4->setText("");
        break;
    case 2:
        ui->statusLabel_1->setText("");
        ui->statusLabel_2->setText("√");
        ui->statusLabel_3->setText("");
        ui->statusLabel_4->setText("");
        break;
    case 3:
        ui->statusLabel_1->setText("");
        ui->statusLabel_2->setText("");
        ui->statusLabel_3->setText("√");
        ui->statusLabel_4->setText("");
        break;
    case 4:
        ui->statusLabel_1->setText("");
        ui->statusLabel_2->setText("");
        ui->statusLabel_3->setText("");
        ui->statusLabel_4->setText("√");
        break;
    default:
        qDebug()<<"error";
    }
}


//绘制主画面
void MainWindow::drawMLCView(){
    // 创建 QLabel 并设置属性
    // QLabel *imageLabel = new QLabel(ui->showPicWidget);
    // imageLabel->setObjectName("imageLabel");
    // imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // imageLabel->setAlignment(Qt::AlignCenter); // 设置居中对齐

    // // 创建布局并将 QLabel 添加到布局中
    // QVBoxLayout *layout = new QVBoxLayout(ui->showPicWidget);
    // layout->addWidget(imageLabel);
    // ui->showPicWidget->setLayout(layout); // 设置 showPicWidget 的布局

    // // 加载高清图片
    // QPixmap pixmap("D:/zm/dcm/bigView.png"); // 替换为你的高清图片路径
    // imageLabel->setPixmap(pixmap.scaled(ui->showPicWidget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}


void MainWindow::on_actiongu_triggered()
{
    QMessageBox::information(this, "帮助", "\n\n\n\n        关于多叶准直器MLC控制系统软件V1.0,本系统目的为了解析dcm文件，并将dcm文件可视化，以实现对多页准直器光栅精准控制的目的。\n\n"
                                           "该系统版权隶属山东亚历山大智能科技有限公司，详情请联系管理员。\n\n\n"
                                           "关于山东亚历山大智能科技有限公司：\n\n"
                                           "地址：山东省济南市历下区经十路华特广场B410\n"
                                           "服务热线：13065039691\n"
                                           "工作时间：周一至周日9:00-22:00\n"
                                           "邮箱：ale-service@alesmart.com\n\n\n"
                                           "备案号：鲁ICP备17053761号-1\n\n");
}


void MainWindow::on_actionban_triggered()
{
    // 打开外部网站
    QDesktopServices::openUrl(QUrl("https://www.alesmart.com/"));
}


void MainWindow::on_actionsss_triggered()
{
    setRunStatus(1);
    QMessageBox::information(this,"提示","设备连接成功");

}


void MainWindow::on_actionre_triggered()
{
    setRunStatus(2);
    QMessageBox::information(this,"提示","设备初始化成功");
}


void MainWindow::on_action_triggered()
{
    setRunStatus(3);
    QMessageBox::information(this,"提示","设备启动成功");
    setRunStatus(4);
}


void MainWindow::on_actionstop_triggered()
{
    setRunStatus(0);
    QMessageBox::warning(this,"提示","停止设备!");
}


void MainWindow::on_actionss_triggered()
{
    setRunStatus(0);
    QMessageBox::warning(this,"提示","断开设备连接!");
}


//处理文件名字
void MainWindow::split_file_name(const QString &filename){
    QStringList parts = filename.split('-');

    if (parts.size() == 6) {
        QString part1 = parts[0];                     // C0001-1
        QString part2 = parts[1];                      // liuaiqin
        QString part3 = parts[2];                      // C000379681
        QString part4 = parts[3];                      // Static
        QString part5 = parts[4];                      // XH600D

        // 将 part2 进行空格分隔处理
        QString formattedPart2 = part2.mid(0, 3) + " " + part2.mid(3); // liu ai qin

        // 输出结果
        qDebug() << part1;
        qDebug() << formattedPart2;
        qDebug() << part3;
        qDebug() << part4;
        qDebug() << part5;
        //设置文件信息 -应该是从名字里面解析
        ui->planNameValue->setText(part1);
        ui->patientNameValue->setText(formattedPart2);
        ui->recordIndexValue->setText(part3);
        ui->treatModeValue->setText(part4);
        ui->machineIndexValue->setText(part5);
        ui->machineAngleValue->setText("0.0");
        ui->MLCAngleValue->setText("0.0");
    } else {
        qDebug() << "分割后部分数量不匹配！";
    }


}
//加载数据和视野
void MainWindow::get_data_and_view(const QString &filepath){
    loaddata(filepath); //读取文件

    QPixmap pixmap(":/pic/D:/zm/dcm/secondView.png");  // 从资源中加载图片
    ui->secondViewLabel->setPixmap(pixmap);  // 设置图片到 QLabel
    ui->secondViewLabel->setScaledContents(true);  // 使图片适应 QLabel 的大小
    ui->secondViewLabel->resize(pixmap.size());  // 根据图片大小调整 QLabel 大小
    ui->secondViewLabel->show();  // 显示 QLabel
    QPixmap pixmap2(":/pic/D:/zm/dcm/bigView.png");  // 从资源中加载图片
    ui->imageLabel->setPixmap(pixmap2);  // 设置图片到 QLabel
    ui->imageLabel->setScaledContents(true);  // 使图片适应 QLabel 的大小
    ui->imageLabel->resize(pixmap2.size());  // 根据图片大小调整 QLabel 大小
    ui->imageLabel->show();  // 显示 QLabel
}
//登录界面
void MainWindow::showLoginDialog(){
    LoginWidget *loginWidget = new LoginWidget(this);  // 创建 LoginWidget 实例

    // 连接登录成功信号
    connect(loginWidget, &LoginWidget::loginSuccessful, this, &MainWindow::show);

    // 连接关闭信号，确保关闭时删除登录窗口
    connect(loginWidget, &QObject::destroyed, this, &MainWindow::show);

    loginWidget->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动删除
    loginWidget->show();  // 显示登录窗口

    // 当登录窗口关闭时，显示主窗口
    connect(loginWidget, &LoginWidget::loginSuccessful, this, [&]() {
        this->setVisible(true);  // 登录成功后显示主窗口
        loginWidget->close();  // 关闭登录窗口
    });
}
