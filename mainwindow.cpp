#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

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




void MainWindow::on_openFolder_triggered()
{
    QFileDialog dialog(nullptr, "请选择Dicom文件", QDir::homePath(), "DCM Files (*.dcm)");
    dialog.setFileMode(QFileDialog::ExistingFiles); // 允许选择多个文件
    dialog.setNameFilter("DCM Files (*.dcm)"); // 设置过滤器以仅显示.dcm文件

    if (dialog.exec() != QDialog::Accepted)
    {
        // 用户取消了文件选择
        return;
    }

    QStringList filepaths = dialog.selectedFiles(); // 获取用户选择的文件路径列表

    // 遍历文件路径列表并输出文件名
    for (const QString &filepath : filepaths)
    {
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


void MainWindow::on_clickExit_triggered()
{
    this->close();
}

