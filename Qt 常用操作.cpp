//Qt 常用操作
#include <QtGlobal> 

//系统平台
#ifdef Q_OS_MAC // mac 
#endif 

#ifdef Q_OS_LINUX // linux 
#endif 

#ifdef Q_OS_WIN32 // win 
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
#include <QtWebEngineWidgets>
#else
#include <QtWebKitWidgets>
#endif

//Qt版本
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets>
#else
#include <QtGui>
#endif

//含有中文，前面一律加 u8
u8"select *from text.case where name='张三'   "

include(Commons/Commons.pri)
include(SaveLog/SaveLog.pri)

DESTDIR = bin #生成目标的路径
MOC_DIR = bin/moc #指定moc命令将含Q_OBJECT的头文件转换成标准.h文件的存放目录
OBJECTS_DIR = bin/obj #指定目标文件(obj)的存放目录
RCC_DIR = bin/qrc #指定rcc命令将.qrc文件转换成qrc_*.h文件的存放目录
UI_DIR = bin/ui #指定uic命令将.ui文件转化成ui_*.h文件的存放的目录

//pro 工程文件添加quazip链接库

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/quazip/lib/ -lquazip
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/quazip/lib/ -lquazipd
else:unix: LIBS += -L$$PWD/quazip/lib/ -lquazip

INCLUDEPATH += $$PWD/quazip/include
DEPENDPATH += $$PWD/quazip/include

//pro 工程文件添加poppler链接库

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/poppler-qt/lib/ -llibpoppler-qt5
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/poppler-qt/lib/ -llibpoppler-qt5
else:unix: LIBS += -L$$PWD/poppler-qt/lib/ -llibpoppler-qt5

INCLUDEPATH += $$PWD/poppler-qt/include/poppler-qt5
INCLUDEPATH += $$PWD/poppler-qt/include/poppler-cpp
DEPENDPATH += $$PWD/poppler-qt/include/poppler-qt5
DEPENDPATH += $$PWD/poppler-qt/include/poppler-cpp

//pro 工程文件添加tidy链接库
unix|win32: LIBS += -L$$PWD/tidy/lib/ -ltidy

INCLUDEPATH += $$PWD/tidy/include
DEPENDPATH += $$PWD/tidy/include

//pro 工程文件链接pri文件
include(NetworkManager/NetworkManager.pri)

//进度条文字居中
QProgressBar *progressBar = new QProgressBar();
progressBar->setAlignment(Qt::AlignLeading | Qt::AlignLeft | Qt::AlignVCenter);

include(../../Utils/Commons/Commons.pri)
INCLUDEPATH += ../../Utils/Commons

include(../../Utils/SaveLog/SaveLog.pri)
INCLUDEPATH += ../../Utils/SaveLog

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Utils/quazip/lib/ -llibquazip1-qt5
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Utils/quazip/lib/ -llibquazip1-qt5d

INCLUDEPATH += $$PWD/../../Utils/quazip/include
DEPENDPATH += $$PWD/../../Utils/quazip/include

//日志（不输出debug）
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
        break;
    case QtInfoMsg:
        text = QString("Info:");
        break;
    }

    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString message;
    if(text != QString("Debug:"))
        message = QString("%1 %2 %3 (%4)").arg(text).arg(context_info).arg(msg).arg(current_date_time);

    if(!message.isEmpty()) {
        QFile file(QStringLiteral("错误日志.log"));
        file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
        QTextStream text_stream(&file);
        text_stream << message << endl;
        file.flush();
        file.close();
    }

    mutex.unlock();
}

qInstallMessageHandler(outputMessage);

//设置配置文件名
a.setOrganizationName("managementSystem");

//main.cpp 加载翻译文件

    QTranslator tl;
    tl.load("://zh_CN");
    a.installTranslator(&tl);

    QTranslator tl2;
    tl2.load("://qt_zh_CN");
    a.installTranslator(&tl2);
	

//设置文件读写权限
QFile::setPermissions(filePath, QFile::ReadOwner | QFile::WriteOwner);

// 设置文件读写权限
bool CommonsQt::setFilePermissions(QString filePath)
{
    QFileDevice::Permissions flags =
            QFileDevice::ReadOwner | QFileDevice::WriteOwner
            | QFileDevice::ReadUser | QFileDevice::WriteUser
            | QFileDevice::ReadGroup | QFileDevice::WriteGroup
            | QFileDevice::ReadOther | QFileDevice::WriteOther;
    return QFile::setPermissions(filePath, flags);
}


#include "Commons.h"
#include "savelog.h"
#include "highlighter.h"

#include "JlCompress.h"

#include "compressEpub.h"

#pragma execution_character_set("utf-8")

//选择文件
private Q_SLOTS:
    void onSelect();

private:
    void setSaveLog();
    QString lastPath;
    Highlighter *highlighter;
    void loadFiles(const QStringList &fileList);
    void loadFile(const QString &filePath);
	
	void startFiles();
	
	void writeText(QString text);
    void writeText(qint64 value);
    void writeMsg(QString text, int msecs = 10000);
    void timeConsuming(int msecs);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

setAcceptDrops(true);
ui->plainTextEdit->setAcceptDrops(false);
connect(ui->selectBtn, &QPushButton::clicked, this, &readEpub::onSelect);
connect(ui->selectAct, &QAction::triggered, this, &readFile::onSelect);
setStyleSheet(QString("font-family: Microsoft Yahei;"));
statusBar()->setStyleSheet(QString("color: blue; font-size: 14px;"));
highlighter = new Highlighter(ui->plainTextEdit->document());
setSaveLog();
startFiles();

void readFile::setSaveLog()
{
    SaveLog::Instance()->setMaxRow(0);
    SaveLog::Instance()->setMaxSize(0);
    SaveLog::Instance()->setUseContext(true);
    SaveLog::Instance()->setPath(QString("%1/logs").arg(qApp->applicationDirPath()));
    SaveLog::Instance()->start();
}

void readFile::startFiles()
{
    QStringList fileList = QCoreApplication::arguments();
    if(fileList.size() < 2)
        return;
    fileList = fileList.mid(1);
    loadFiles(fileList);
}

void readFile::onSelect()
{
	//选择单文件
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("选择epub文件"),
                                                    lastPath,
                                                    tr("epub文件(*.epub)"));
    if(filePath.isEmpty())
        return;
    loadFile(filePath);
	
	//选择多文件
	QStringList fileList = QFileDialog::getOpenFileNames(this,
                                                         tr("选择文件"),
                                                         lastPath,
                                                         tr("zip文件(*.zip);;所有文件(*.*)"));
    if(fileList.isEmpty())
        return;

    loadFiles(fileList);
}

void readFile::loadFiles(const QStringList &fileList)
{
	//操作多文件……
    QTime timeUp;
    timeUp.start();
    statusBar()->showMessage(u8"开始……");

    foreach (QString filePath, fileList)
        loadFile(filePath);

    statusBar()->showMessage(u8"完毕", 10000);
    timeConsuming(timeUp.elapsed());
}

void readFile::loadFile(const QString &filePath)
{
	//操作单文件……
}

void readFile::writeText(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}

void readFile::writeText(qint64 value)
{
    ui->plainTextEdit->appendPlainText(QString::number(value));
}

void readFile::writeMsg(QString text, int msecs)
{
    ui->plainTextEdit->appendPlainText(text);
    statusBar()->showMessage(text, msecs);
}

void readFile::timeConsuming(int msecs)
{
    double secs = msecs / 1000.0;
    QString timeStr = secs < 60 ? QString("%1 秒").arg(secs)
                                : Commons::timeFormat(static_cast<int>(secs));
    writeText(QString("共计耗时 %1\n").arg(timeStr));
}

void readFile::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

//拖放单文件
void readFile::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
    QString filePath;

    foreach (QUrl url, mime->urls()) {
        QString fileName(url.toLocalFile());
        QFileInfo fi(fileName);
        if(fi.isFile() && fileName.endsWith(".epub", Qt::CaseInsensitive)) {
            filePath = fileName;
            break;
        }
    }
    if(filePath.isEmpty())
        return;

    loadFile(filePath);
}

    //字符串按照拼音排序
static QStringList pinYinList(QStringList list) {
    QLocale cn(QLocale::Chinese);
    QCollator collator(cn);
    qStableSort(list.begin(), list.end(), collator);
//    qDebug() << "顺序列表：" << list;
    return list;
}

void readFile::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

//拖放多文件
void readFile::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
    QList<QUrl> duplicates;
    QStringList fileList;

    foreach (QUrl url, mime->urls()) {
        if (duplicates.contains(url))
            continue;
        else
            duplicates << url;
        QString filePath(url.toLocalFile());
        QFileInfo fi(filePath);
        if(fi.isFile())
            fileList << filePath;
    }
    fileList = Commons::pinYinList(fileList);

    if(fileList.isEmpty())
        return;
    loadFiles(fileList);
}

public Q_SLOTS:
	void onCopyText();
    void onCopyPath();
    void onOPenFile();
    void onOpenFolder();

    void itemReceiver(QTableWidgetItem *item);
	
private:
    void setTableWidget();
    void createContextMenu();
    QMenu *popMenu;
    QAction *copyTextAct;
    QAction *copyPathAct;
    QAction *openFileAct;
    QAction *openFolderAct;



//设置表格
void setTableWidget()
{
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(50);
    ui->tableWidget->setColumnCount(6); //设置列数
    //设置表头内容
    QStringList headers;
    headers << QStringLiteral("文件名") << QStringLiteral("文件路径") << QStringLiteral("大小（bytes）")
            << QStringLiteral("大小") << QStringLiteral("类型") << QStringLiteral("信息");

    ui->tableWidget->setHorizontalHeaderLabels(headers);

//    ui->tableWidget->setColumnWidth(0, 150);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget->setColumnHidden(1, true);
    ui->tableWidget->setColumnHidden(2, true);
    ui->tableWidget->setColumnHidden(4, true);

    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(3, 100);

    ui->tableWidget->verticalHeader()->setVisible(true);

    connect(ui->tableWidget, &myTableWidget::itemSender, this, &readEpub::itemReceiver);
    connect(ui->tableWidget, &myTableWidget::itemDoubleClicked, this, &readEpub::onOPenFile);
}

void createContextMenu()
{
    popMenu = new QMenu(tr("右键菜单"),  this);
    copyTextAct = new QAction(tr("复制文本"), this);;
    copyPathAct = new QAction(tr("复制文件路径"), this);
    openFileAct = new QAction(tr("打开文件"), this);
    openFolderAct = new QAction(tr("打开所在文件夹"), this);

    connect(copyTextAct, &QAction::triggered, this, &readEpub::onCopyText);
    connect(copyPathAct, &QAction::triggered, this, &readEpub::onCopyPath);
    connect(openFileAct, &QAction::triggered, this, &readEpub::onOPenFile);
    connect(openFolderAct, &QAction::triggered, this, &readEpub::onOpenFolder);
}

//当前单元格文本复制到剪贴板
QString myTableWidget::currentItemText()
{
    QTableWidgetItem *item = currentItem();
    if(item) {
        QString text = item->text();
        if(text.isEmpty())
            return "";
        QClipboard *board = QApplication::clipboard();
        board->setText(text);
        return text;
    }
    return "";
}

//当前行指定列单元格文本复制到剪贴板
QString myTableWidget::columnItemText(int column)
{
    QTableWidgetItem *cItem = currentItem();
    QTableWidgetItem *item = this->item(cItem->row(), column);
    if(item) {
        QString text = item->text();
        if(text.isEmpty())
            return "";
        QClipboard *board = QApplication::clipboard();
        board->setText(text);
        return text;
    }
    return "";
}

//复制当前单元格文本
void readEpub::onCopyText()
{
    QString text = ui->tableWidget->currentItemText();
    if(!text.isEmpty())
        statusBar()->showMessage(tr("“%1”已复制到粘贴板").arg(text), 10000);
}

//复制当前文件路径
void readEpub::onCopyPath()
{
    QString text = ui->tableWidget->columnItemText(1);
    if(!text.isEmpty())
        statusBar()->showMessage(tr("“%1”已复制到粘贴板").arg(text), 10000);
}

//打开文件
void readEpub::onOPenFile()
{
    QTableWidgetItem *item = ui->tableWidget->currentItem();
    QString fileName = ui->tableWidget->item(item->row(), 0)->text();
    QString filePath = ui->tableWidget->item(item->row(), 1)->text();

    QByteArray ba = QUrl::toPercentEncoding(filePath);
    bool ok = QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(QString(ba))));
    if(!ok)
        statusBar()->showMessage(tr("%1 文件不存在").arg(fileName), 10000);
}

//打开文件所在文件夹
void readEpub::onOpenFolder()
{
    QTableWidgetItem *item = ui->tableWidget->currentItem();
    QString fileName = ui->tableWidget->item(item->row(), 0)->text();
    QString filePath = ui->tableWidget->item(item->row(), 1)->text();
    if(!QFile::exists(filePath)) {
        statusBar()->showMessage(tr("%1 文件不存在").arg(fileName), 10000);
        return;
    }
    filePath.replace("/", "\\");
    QProcess proc(this);
    QString cmd("explorer.exe");
    QStringList argList;
    argList << QString("/select,") << filePath;
    qDebug() << "打开文件：" << cmd << argList;
    proc.startDetached(cmd, argList);
}

    void openCurrentFile(QString filePath);
    void openCurrentFolder(QString filePath);

//打开文件
void countQingJian::onOPenFile()
{
    QTableWidgetItem *item = ui->tableWidget->currentItem();
    QString filePath = ui->tableWidget->item(item->row(), 1)->text();
    openCurrentFile(filePath);
}

void countQingJian::openCurrentFile(QString filePath)
{
    QString fileName = QFileInfo(filePath).fileName();
    QByteArray ba = QUrl::toPercentEncoding(filePath);
    bool ok = QDesktopServices::openUrl(QUrl(QString("file:///%1").arg(QString(ba))));
    if(!ok)
        statusBar()->showMessage(tr("%1 文件不存在").arg(fileName), 10000);
}

//打开文件所在文件夹
void countQingJian::onOpenFolder()
{
    QTableWidgetItem *item = ui->tableWidget->currentItem();
    QString filePath = ui->tableWidget->item(item->row(), 1)->text();
    openCurrentFolder(filePath);
}

void countQingJian::openCurrentFolder(QString filePath)
{
    QString fileName = QFileInfo(filePath).fileName();
    if(!QFile::exists(filePath)) {
        statusBar()->showMessage(tr("%1 文件不存在").arg(fileName), 10000);
        return;
    }
    filePath.replace("/", "\\");
    QProcess proc(this);
    QString cmd = QString("explorer.exe /select,%1").arg(filePath);
    qDebug() << "打开文件：" << cmd;
    proc.startDetached(cmd);
}

void readEpub::itemReceiver(QTableWidgetItem *item)
{
    QTableWidget *myTable = qobject_cast<QTableWidget *>(sender());
    popMenu->clear();
    QString objName = myTable->objectName();

    if(objName == "tableWidget") {
        if(item) {
            popMenu->addAction(openFileAct);
            popMenu->addAction(openFolderAct);
            popMenu->addAction(copyTextAct);
            popMenu->addAction(copyPathAct);
            popMenu->exec(QCursor::pos());
        } else {
        }
    }
}

//QTableWidgetItem 设置图标
        ui->tableWidget->setRowCount(errorPairList.size());
        for(int i = 0; i < errorPairList.size(); i++) {
            auto pair = errorPairList.at(i);
            QString bookPath = pair.first;
            QString errorStr = pair.second;
            QFileInfo info(bookPath);
            QString fileName = info.fileName();
            qint64 size = info.size();
            QString sizeStr = Commons::byteUnit(size);
            QString typeStr = info.suffix().toLower();
            QStringList list;
            list << fileName << bookPath << QString::number(size) << sizeStr
                 << typeStr << errorStr;
            for(int col = 0; col < list.size(); col++) {
                if(col == 0) {
                    QFileIconProvider icon_provider;
                    QIcon icon = icon_provider.icon(QFileInfo(list.at(1)));
                    QTableWidgetItem *item = ui->tableWidget->itemSetting(i, col, list.at(col));
                    item->setIcon(icon);
                    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                } else {
                    ui->tableWidget->itemSetting(i, col, list.at(col));
                }
            }
        }

//QTableWidgetItem 设置不可编辑	
item->setFlags(item->flags() & (~Qt::ItemIsEnabled));
//QTableWidgetItem 设置颜色
item->setTextColor(Qt::blue);
		
//不解压读zip/epub
    QString ncxText;
    QuaZip zip(epubPath);
    zip.open(QuaZip::mdUnzip);
    QuaZipFile file(&zip);
    for(bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
        file.open(QIODevice::ReadOnly);
        qDebug() << file.size() << file.getActualFileName();
        QString name = file.getActualFileName();
        if(name.endsWith(".ncx", Qt::CaseInsensitive)) {
            ncxText = QString(file.readAll());
            break;
        }
        file.close();
    }
    zip.close();
	
	

include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)

//读xlsx
    QFileInfo info(filePath);
    QString fileName = info.fileName();
    QXlsx::Document xlsx(filePath);
    QXlsx::CellRange cellRange = xlsx.dimension();
    int row_count = cellRange.rowCount();
    int column_count = cellRange.columnCount();
    qDebug() << fileName << row_count << column_count;

    for(int i = 1; i < row_count + 1; i++) {
        for(int j = 1; j < column_count + 1; j++) {
            QXlsx::Cell *cell = xlsx.cellAt(i, j);
            if(!cell)
                continue;
            QVariant value = cell->value();
            qDebug() << QString("第%1行 第%2列").arg(i).arg(j) << value << value.toString();
        }
    }

//通用样式格式
QXlsx::Format commonFormat(QFont font)
{
    QXlsx::Format format;
    format.setHorizontalAlignment(QXlsx::Format::HorizontalAlignment::AlignHCenter);
    format.setVerticalAlignment(QXlsx::Format::VerticalAlignment::AlignVCenter);
    format.setTextWarp(true);
    format.setBorderStyle(QXlsx::Format::BorderThin);
    format.setFont(font);
    return format;
}


QT += axcontainer
#include "QtExceL.h"

//xls转xlsx
    if(info.suffix().toLower() == "xls") {
        QtExcel excel(filePath);
        if(excel.Open()) {
            QString path = QString("%1x").arg(filePath).replace("/", "\\");
            excel.SaveAs(path, false);
            filePath = path;
            filePath.replace("\\", "/");
            spePath = filePath;
        } else {
            writeMsg(tr("错误：%1 无法打开").arg(fileName));
            return;
        }
    }
	

//Dialog show
Dialog->setAttribute(Qt::WA_DeleteOnClose);

QList<rotateDialog *> list = this->findChildren<rotateDialog *>();
if(!list.isEmpty())
    return;
rotateDialog *rotateDlg = new rotateDialog(this);
connect(rotateDlg, &rotateDialog::angleChanged, this, &tiltCorrection::onAngleChanged);
rotateDlg->show();

//调用cmd
private slots:
    void onReadyReadStandardError();
    void onReadyReadStandardOutput();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);


void epubCheck::loadFile(QString filePath)
{
    ui->lineEdit->setText(filePath);
    xmlHighlighter = new XmlSyntaxHighlighter(ui->plainTextEdit->document(), filePath);
    statusBar()->showMessage(tr("正在检测epub……"));
    ui->selectBtn->setEnabled(false);
    ui->clearBtn->setEnabled(false);
    QString jarFile = QString("%1/epubcheck/epubcheck.jar").arg(QDir::currentPath());
    qDebug() << jarFile;
    QProcess *proc = new QProcess(this);
    QStringList list = QStringList(filePath);
    proc->start(QString("java -jar \"%1\"").arg(jarFile), list);

    connect(proc, &QProcess::readyReadStandardError, this, &epubCheck::onReadyReadStandardError);
    connect(proc, &QProcess::readyReadStandardOutput, this, &epubCheck::onReadyReadStandardOutput);
    connect(proc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
 	
	//或者
	proc->waitForStarted();
    proc->waitForFinished();
    QByteArray ba = proc->readAllStandardOutput();
    qDebug() << ba.replace("\r", "").replace("\n", "");
    ui->plainTextEdit->appendPlainText(QString(ba));
}

void epubCheck::onReadyReadStandardError()
{
    QProcess *proc = qobject_cast<QProcess *>(sender());
    QByteArray ba = proc->readAllStandardError();
    ba = Commons::codec1ToCodec2(ba, "GBK");
    ui->plainTextEdit->appendPlainText(QString(ba));
    ui->plainTextEdit->moveTextCursorEnd();
}

void epubCheck::onReadyReadStandardOutput()
{
    QProcess *proc = qobject_cast<QProcess *>(sender());
    QByteArray ba = proc->readAllStandardOutput();
    ba = Commons::codec1ToCodec2(ba, "GBK");
    ui->plainTextEdit->appendPlainText(QString(ba));
    ui->plainTextEdit->moveTextCursorEnd();
}

void epubCheck::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << exitCode << exitStatus;
    statusBar()->showMessage(tr("检测完毕"), 10000);
    ui->selectBtn->setEnabled(true);
    ui->clearBtn->setEnabled(true);
}

    int officialRow;
    int noOfficialRow;
    int boxRow;
    int dossierRow;

    void writeIni();
    void readIni();
	
    void closeEvent(QCloseEvent *event);

// 读取配置文件
void countQingJian::readIni()
{
    QDir dir = QDir::current().absoluteFilePath("config");
    if(!dir.exists())
        QDir::current().mkdir("config");

    QString configPath = dir.absoluteFilePath("config.ini");
    if(!QFile::exists(configPath)) {
        officialRow = 3;
        noOfficialRow = 3;
        boxRow = 4;
        dossierRow = 3;
    } else {
        QSettings settings(configPath, QSettings::IniFormat);
        settings.setIniCodec("UTF-8");
        officialRow = settings.value("catalog/official_row").toInt();
        noOfficialRow = settings.value("catalog/no_official_row").toInt();
        boxRow = settings.value("catalog/box_row").toInt();
        dossierRow = settings.value("catalog/dossier_row").toInt();
    }
    ui->officialSpinBox->setValue(officialRow);
    ui->noOfficialSpinBox->setValue(noOfficialRow);
    ui->boxSpinBox->setValue(boxRow);
    ui->dossierSpinBox->setValue(dossierRow);
}

// 写入配置文件
void countQingJian::writeIni()
{
    QDir dir = QDir::current().absoluteFilePath("config");
    if(!dir.exists())
        QDir::current().mkdir("config");

    QString configPath = dir.absoluteFilePath("config.ini");
    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.beginGroup(QString("catalog"));
    settings.setValue(QString("official_row"), officialRow);
    settings.setValue(QString("no_official_row"), noOfficialRow);
    settings.setValue(QString("box_row"), boxRow);
    settings.setValue(QString("dossier_row"), dossierRow);
    settings.endGroup();
}

void countQingJian::closeEvent(QCloseEvent *event)
{
    writeIni();
    event->accept();
}

    QFileInfo file_info(filePath);
    QFileIconProvider icon_provider;
    QIcon icon = icon_provider.icon(file_info);
	
    QFileIconProvider icon_provider;
    QIcon icon = icon_provider.icon(QFileIconProvider::Folder);
	
	QFileIconProvider::Computer
	QFileIconProvider::Desktop
	QFileIconProvider::Trashcan
	QFileIconProvider::Network
	QFileIconProvider::Drive
	QFileIconProvider::Folder
	QFileIconProvider::File
	

// 辗转相除法求两个数的最大公约数
int simplegcd(int a, int b) {
    return b == 0 ? a : simplegcd(b, a % b);
}

// 最小公倍数 = 两数之积除以最大公约数
int simplelcm(int a, int b) {
	return a * b / simplegcd(a, b);
}

QImage imageFromPath(QString imagePath)
{
    QImageReader imageReader(imagePath);
    imageReader.setAutoTransform(true);
    imageReader.setDecideFormatFromContent(true);
    QImage image = imageReader.read();
    return image;
}

// Qt 屏蔽输入法
QLineEdit lineEditPwd;
lineEditPwd.setAttribute(Qt::WA_InputMethodEnabled, false); 

密码框过滤：[a-zA-Z0-9@#\$%\^&\*\(\)~!\{\}:\"<>\?_+\|`\[\];',\.\/\-\=\\]
// 设置密码输入框
static void setPwdEdit(QLineEdit *edit)
{
    edit->setAttribute(Qt::WA_InputMethodEnabled, false);
    QRegularExpression reg("[a-zA-Z0-9@#\\$%\\^&\\*\\(\\)~!\\{\\}:\"<>\\?_\\+\\|`\\[\\];',\\.\\/\\-=\\\\]+");
    edit->setValidator(new QRegularExpressionValidator(reg));
}

// winrar解压缩
QString rarPath = QDir::current().absoluteFilePath("Rar.exe");
if(!QFile::exists(rarPath)) {
	ui->plainTextEdit->appendPlainText(tr("%1 提取epub文件失败（未检测到rar.exe文件）").arg(fileName));
} else {
	QStringList list;
	list << "e" << "-or" << "-ibck" << filePath << "*.epub" << epubDir.path();
	QProcess::startDetached(rarPath, list);
}

// 获取文件类型/图标
QMimeDatabase mimeDatabase;
QMimeType mimeType = mimeDatabase.mimeTypeForFile(filePath);
const QString &type = mimeType.name();
const QString &comment = mimeType.comment();
const QIcon &fileIcon = QIcon::fromTheme(mimeType.iconName(), QIcon::fromTheme(mimeType.genericIconName()));