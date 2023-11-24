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

CONFIG += utf8_source

// 乱码问题
#include <windows.h>
SetConsoleOutputCP(codePage);
其中codePage为代码页，常见取值为CP_UTF8，代表UTF-8编码。

如果编译器用的是MSVC，需要在pro文件内加入 QMAKE_CXXFLAGS += /utf-8

//含有中文，前面一律加 u8
u8"select *from text.case where name='张三'   "

 properties="duokan-page-fullscreen"

pyinstaller -F -w --icon="logo3.ico" 文件名.py

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

CONFIG += c++11

include(../../Utils/Commons/Commons.pri)
INCLUDEPATH += ../../Utils/Commons

include(../../Utils/SaveLog/SaveLog.pri)
INCLUDEPATH += ../../Utils/SaveLog

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Utils/quazip/lib/ -llibquazip1-qt5
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Utils/quazip/lib/ -llibquazip1-qt5d

INCLUDEPATH += $$PWD/../../Utils/quazip/include
DEPENDPATH += $$PWD/../../Utils/quazip/include

include(../../Utils/CommonsLite/CommonsLite.pri)
INCLUDEPATH += ../../Utils/CommonsLite

include(../../Utils/MakePath/MakePath.pri)
INCLUDEPATH += ../../Utils/MakePath

include(../../Utils/translation/translation.pri)
INCLUDEPATH += ../../Utils/translation

include(../../Utils/SaveLog/SaveLog.pri)
INCLUDEPATH += ../../Utils/SaveLog

include(../../Utils/SingleApplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication

// 静态链接库
QMAKE_CFLAGS_DEBUG += -MTd
QMAKE_CXXFLAGS_DEBUG += -MTd

QMAKE_CFLAGS_RELEASE += -MT
QMAKE_CXXFLAGS_RELEASE += -MT

QMAKE_LFLAGS +=/NODEFAULTLIB:msvcrt

// 应用程序单例:
include(../../Utils/SingleApplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QApplication  // QWidget
或
DEFINES += QAPPLICATION_CLASS=QCoreApplication // Console

// pro里判断当前编译模式
CONFIG += debug_and_release
CONFIG(debug, debug|release){
    contains(DEFINES, WIN64) {
    TARGET = ../../_debug64/AppName
    } else {
    TARGET = ../../_debug32/AppName
    }
} else {
    contains(DEFINES, WIN64) {
    TARGET = ../../_release64/AppName
    } else {
    TARGET = ../../_release32/AppName
    }
}

// 代码里判断当前编译模式
#ifdef QT_NO_DEBUG
    qDebug() << "release mode";
#else
    qDebug() << "debug mode";
#endif

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

// lambda函数
QPushButton *btn2 = new QPushButton;
btn2->move(0,0);
btn2->setParent(this);
btn2->setText("aaa");

[=](){
   btn2->setText("bbb");
}();

auto intToString = [=](QList<int> intList) -> QStringList {
	QStringList strList;
	for(auto i : intList)
		strList << QString::number(i);
	return strList;
};

//获取随机数（左闭右开[min, max)）
static int randomInt(int min, int max) {
	Q_ASSERT(min < max);
	// 加入随机种子。种子是当前时间距离0点0分0秒的秒数。
	// 每次启动程序，只添加一次种子，以做到数字真正随机。
	static bool seedStatus;
	if (!seedStatus)
	{
		std::srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
		seedStatus = true;
	}
	int nRandom = std::rand() % (max - min);
	nRandom = min + nRandom;

	return nRandom;
}

// 闰年
bool isLeapYear(int year)
{
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

//Qt 数字转字符串，千分位分隔符
QString numberToStr(int k) {
	QString str = QString("%L1").arg(k);
	return str;
}

//Qt 浮点数转字符串，千分位分隔符
QString numberToStr(double k) {
	QString str = QString("%L1").arg(k, 0, 'f', 0);
	return str;
}

// 设置QLineEdit的文字从最左边开始显示（要在setText之后设置）
// 方法一：setSelection
ui->lineEdit->setSelection(0, 0);
// 方法二：setCursorPosition
ui->lineEdit->setCursorPosition(0);

#include "JlCompress.h"

#include "compressEpub.h"

#pragma execution_character_set("utf-8")

#include "Commons.h"
#include "MsgHighlight.h"
#include "logs.h"

//选择文件
private Q_SLOTS:
    void onSelect();

private:
	void setQss();
    void setSaveLog();
    QString lastPath;
    MsgHighlight *highlighter;
    void loadFiles(const QStringList &fileList);
    void loadFile(const QString &filePath);
	
	void startFiles();
	
	void writeText(QString text);
    void writeText(qint64 value);
    void writeMsg(QString text, int msecs = 10000);
    void timeConsuming(qint64 msecs);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

setWindowTitle("title");
setAcceptDrops(true);
ui->plainTextEdit->setAcceptDrops(false);
connect(ui->selectBtn, &QPushButton::clicked, this, &readEpub::onSelect);
connect(ui->selectAct, &QAction::triggered, this, &readFile::onSelect);
setStyleSheet(QString("font-family: Microsoft Yahei;"));
statusBar()->setStyleSheet(QString("color: blue; font-size: 14px;"));
setStyleSheet(QString("* {font-family: Microsoft Yahei;}\n"
					  "QComboBox QAbstractItemView { outline: none; }\n"
					  "QStatusBar { color: blue; font-size: 14px; }"));
highlighter = new MsgHighlight(ui->plainTextEdit);
setQss();
SetSaveLog;
startFiles();

#if QT_NO_DEBUG
    SetSaveLog;
#endif

// debug 列表
template <class T>
inline void debugList(const QList<T> &list) { for(const auto &t : list) qDebug () << t; }

// 设置界面风格
{
	QStringList styles = QStyleFactory::keys(); // "windowsvista"（默认）, "Windows", "Fusion"
	qApp->setStyle(QStyleFactory::create("Fusion");
}

// 设置窗体（常用于QDialog）最大化和最小化
Qt::WindowFlags windowFlag  = Qt::Dialog;
windowFlag                  |= Qt::WindowMinMaxButtonsHint;
windowFlag                  |= Qt::WindowCloseButtonHint;
setWindowFlags(windowFlag);

// 工具栏添加 QWidget
QComboBox *comboBox;
void setComboBox();

void fbdToTxt::setComboBox()
{
    ui->mainToolBar->addSeparator();
    QWidget *widget = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(3, 0, 3, 0);
    QLabel *label = new QLabel("fbd 源文件格式：");
    hLayout->addWidget(label);
    comboBox = new QComboBox();
    comboBox->addItems(QStringList{ "ANSI", "UTF-8" });
    hLayout->addWidget(comboBox);
    widget->setLayout(hLayout);
    ui->mainToolBar->addWidget(widget);
}

void mergeImage::setQss()
{
    QFile file(":/qss");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(QString(file.readAll()));
    file.close();
}

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
                                                         tr("选择epub文件"),
                                                         lastPath,
                                                         tr("epub文件(*.epub);;所有文件(*.*)"));
    if(fileList.isEmpty())
        return;

    loadFiles(fileList);
}

void readFile::loadFiles(const QStringList &fileList)
{
	//操作多文件……
    QElapsedTimer timeUp;
    timeUp.start();
    statusBar()->showMessage("开始……");

    foreach (QString filePath, fileList)
        loadFile(filePath);

    statusBar()->showMessage("完毕", 10000);
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

void readFile::timeConsuming(qint64 msecs)
{
	highlighter->normal(QString("共计用时 %1\n").arg(Commons::timeFormat(msecs)));
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

// 程序编译时间
void onAbout();
void onAboutQt();

connect(ui->aboutAct, &QAction::triggered, this, &QQReaderCheck::onAbout);
connect(ui->aboutQtAct, &QAction::triggered, this, &QQReaderCheck::onAboutQt);

QString QQReaderCheck::compilingDate()
{
    int day = QDate::currentDate().day();
    QString format = day < 10 ? "MMM  d yyyy" : "MMM dd yyyy";
#if 0
    QString dateTime = QString("%1 %2").arg(__DATE__, __TIME__);
    qDebug() << "编译日期" << dateTime;
    QDateTime date = QLocale(QLocale::English)
            .toDateTime(dateTime, QString(format + " hh:mm:ss"));
#else
    QString dateTime = QString("%1").arg(__DATE__);
    qDebug() << "编译日期" << dateTime;
    QDate date = QLocale(QLocale::English).toDate(dateTime, format);
#endif
    QString dateText = QLocale::system().toString(date);
    return dateText;
}

void QQReaderCheck::onAbout()
{
    QString dateText = compilingDate();
    QVersionNumber version(0, 9, 5);
    QString title = windowTitle();
    qDebug() << dateText << qVersion() << title << version.toString();
    QString msg = QString("<p><b>%1 v%2 <b/></p>"
                          "<p>%1是一个基于 <a style=\"text-decoration:none;\" "
                          "href=\"https://www.qt.io/\">Qt %3</a>，"
                          "使用 <a style=\"text-decoration:none;\" "
                          "href=\"https://github.com/stachenov/quazip/\">"
                          "%4</a>，检测QQ阅读epub内容是否符合规范的简易工具。</p>"
                          "<p>&copy; 信息技术 - %5</p>")
            .arg(title).arg(version.toString()).arg(qVersion()).arg("QuaZip").arg(dateText);
    QMessageBox::about(this, "关于" + title, msg);
}

void QQReaderCheck::onAboutQt()
{
    QMessageBox::aboutQt(this, "关于 Qt");
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
    copyTextAct = new QAction(tr("复制文本"), this);
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

// QTableWidget设置部分单元格可编辑

// 1.先设置全局编辑属性（单元格焦点改变时可编辑）
ui->tableWidget->setEditTriggers(QAbstractItemView::CurrentChanged);

// 2.设置不可编辑的单元格，剩下的就是可编辑的
QTableWidgetItem *item = ...;
item->setFlags(item->flags() & (~Qt::ItemIsEditable));

//QTableWidgetItem 设置不可编辑	
item->setFlags(item->flags() & (~Qt::ItemIsEnabled)); // ItemIsEnabled 会使单元格变成灰色
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

// lambda传值可修改
std::function<int(int)> foo{[kk](int x) mutable {
    kk.push_back(1);
    return kk[0]+1;
}};


void imagePreviewDialog::keyPressEvent(QKeyEvent *event)
{
    Qt::KeyboardModifiers modifier = event->modifiers();
    bool ctrl = modifier == Qt::ControlModifier;
    if(ctrl) {
        if(event->key() == Qt::Key_1)
            graphicsWidget->fitPageSize();
        if(event->key() == Qt::Key_2)
            graphicsWidget->fitPageWith();
        if(event->key() == Qt::Key_3)
            graphicsWidget->originalSize();
    } else {
        if(event->key() == Qt::Key_Escape)
            close();
    }
    event->accept();
}

//MAC地址
QString LoginWindow::addressMAC()
{
    QString macAddress;
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        if(netInterface.isValid()
                && netInterface.flags().testFlag(QNetworkInterface::IsUp)
                && netInterface.flags().testFlag(QNetworkInterface::IsRunning)
                && netInterface.flags().testFlag(QNetworkInterface::CanBroadcast)
                && netInterface.flags().testFlag(QNetworkInterface::CanMulticast)
                && !netInterface.flags().testFlag(QNetworkInterface::IsLoopBack)
                && netInterface.hardwareAddress() != "00:50:56:C0:00:01"
                && netInterface.hardwareAddress() != "00:50:56:C0:00:08"
                && (netInterface.humanReadableName().contains("WLAN", Qt::CaseInsensitive)
                    || netInterface.humanReadableName().contains("以太网")
                    || netInterface.humanReadableName().contains("ethernet")
                    || netInterface.humanReadableName().contains("无线")
                    || netInterface.humanReadableName().contains("本地连接"))
                && !netInterface.humanReadableName().contains("VMware", Qt::CaseInsensitive)
                && !netInterface.humanReadableName().contains("VirtualBox", Qt::CaseInsensitive)
                && !netInterface.humanReadableName().contains("isatap", Qt::CaseInsensitive)
                && !netInterface.humanReadableName().contains("Loopback", Qt::CaseInsensitive)
                && !netInterface.humanReadableName().contains("Teredo", Qt::CaseInsensitive)) {
            QList<QNetworkAddressEntry> entryList = netInterface.addressEntries();

            QHostAddress addr;
            //遍历每一个IP地址(每个包含一个IP地址，一个子网掩码和一个广播地址)
            foreach(QNetworkAddressEntry entry, entryList) {
                QHostAddress address = entry.ip();
                if (address != QHostAddress::LocalHost
                        && address.protocol() == QAbstractSocket::IPv4Protocol
                        && address.toString().startsWith("192.168"))
                    addr = address;
            }
            if(!addr.isNull())
                macAddress = netInterface.hardwareAddress();
        }
    }
    return macAddress;
}
// 从剪贴板获取 来源：1.复制文件；2.复制图片
void imageColorExtraction::onMimePasted()
{
    QClipboard *board = QApplication::clipboard();
    const QMimeData * mimeData = board->mimeData();
    if(mimeData->hasUrls()) {
        QString filePath;
        foreach (QUrl url, mimeData->urls()) {
            QString path(url.toLocalFile());
            QFileInfo fi(path);
            if(fi.isFile() && m_imgFmtList.contains("*." + fi.suffix().toLower())) {
                filePath = path;
                break;
            }
        }
        if(filePath.isEmpty())
            return;
        loadImage(filePath);
    } else if(mimeData->hasImage()) {
        QImage image = board->image();
        if(image.isNull())
            return;
        graphicsWidget->showImage(image);
        QString pixel = pixelText(image);
        QString title = QString("图片来自剪贴板 | %1 - 色彩提取").arg(pixel);
        setWindowTitle(title);
        setColorCube(image);
        mImage = image;
    }
}

// 图片保存
void imageColorExtraction::onImageSaved()
{
    if(mImage.isNull())
        return;
    QDir dir = QCoreApplication::applicationDirPath();
    QString name = "save";
    QDir saveDir = dir.absoluteFilePath(name);
    if(!saveDir.exists())
        dir.mkdir(name);
    QString currentTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString imageName = QString("ICM_%1.png").arg(currentTime);
    QString imagePath = saveDir.absoluteFilePath(imageName);
    mImage.save(imagePath);
}

// 图片另存为
void imageColorExtraction::onImageSavedAs()
{
    QString filter = QString("PNG(*.png);;JPEG(*.jpg *.jpeg);;BMP(*.bmp)");
    QString currentTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QString imageName = QString("ICM_%1.png").arg(currentTime);
    QString imagePath = QFileDialog::getSaveFileName(this,
                                                    "图片另存为",
                                                    imageName,
                                                    filter);
    if(imagePath.isEmpty())
        return;
    mImage.save(imagePath);
}

// QLayoutItem 在 QLayout 中的索引（Qt5.12之下）
int indexOfLayout(QLayout *layout, QLayoutItem *layoutItem)
{
    int i = 0;
    QLayoutItem *item = layout->itemAt(i);
    while (item) {
        if (item == layoutItem)
            return i;
        ++i;
        item = layout->itemAt(i);
    }
    return -1;
}

ip:
^((25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))\.){3}(25[0-5]|2[0-4]\d|((1\d{2})|([1-9]?\d)))$

port:
1-65535
^([0-9]{1,4}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$


tableView->setIndexWidget(m_model->index(0, 1), new QPushButton("abcd"));

// Qt 多线程
// 一、QThreadPool 线程池 QRunnable
class poolManager : public QObject
class taskWorker : public QObject, public QRunnable

// 主程序调用
void mergeImage::setPoolManager(QStringList fileList, bool isBatch)
{
    qDebug() << "main process" << QThread::currentThreadId();
    QThread *worker_thread = new QThread;
    poolManager *manager = new poolManager(ui->threadSpinBox->value());
    manager->setParams(fileList,
                       ui->directCbB->currentIndex(),
                       ui->imgCbB->currentText(),
                       ui->moreWidget->isVisible(),
                       ui->folderEdit->text(),
                       ui->scaleCbB->currentIndex(),
                       ui->scaleSpinBox->value(),
                       ui->sizeSpinBox->value(),
                       ui->averageSpinBox->value(),
                       isBatch);
    connect(manager, &poolManager::highlightChanged, this, &mergeImage::onHighlightChanged);
    connect(manager, &poolManager::finished, this, &mergeImage::onManagerFinished);
    connect(manager, &poolManager::finished, worker_thread, &QThread::quit);
    connect(worker_thread, &QThread::finished, worker_thread, &QThread::deleteLater);
    manager->moveToThread(worker_thread);
    worker_thread->start();
    manager->poolStart();
}

void mergeImage::onHighlightChanged(HighlightType type, QString msg)
{
    highlighter->appendMsg(type, msg);
}

void mergeImage::onManagerFinished(qint64 msecs)
{
    qDebug() << "结束多线程";
    timeConsuming(msecs);
    setButtonEnable(true);
}

// 二、QtConcurrent
Qt += concurrent

#include <QtConcurrent>
#include <QFutureWatcher>

// 1.run
{
    mergeObject *mergeObj = new mergeObject();
    connect(mergeObj, &mergeObject::highlightChanged, this, &mergeImage::onHighlightChanged);
    mergeObj->setParams(fileList,
                        ui->directCbB->currentIndex(),
                        ui->imgCbB->currentText(),
                        ui->moreWidget->isVisible(),
                        ui->folderEdit->text(),
                        ui->scaleCbB->currentIndex(),
                        ui->scaleSpinBox->value(),
                        ui->sizeSpinBox->value(),
                        ui->averageSpinBox->value());
    QFuture<void> func = QtConcurrent::run(mergeObj, &mergeObject::merge, fileList);
}

// 2.map
// map里面需要放静态函数
{
    QFuture<void> vFuture = QtConcurrent::map(list, mergeObject::test);
    QFutureWatcher<void> *m_pWatcher = new QFutureWatcher<void>(this);
    m_pWatcher->setFuture(vFuture);
    connect(m_pWatcher, &QFutureWatcher<void>::started, [=](){
        qDebug() << "QFutureWatcher started";
    });
    connect(m_pWatcher, &QFutureWatcher<void>::resultReadyAt, [=](int index){
        qDebug() << "QFutureWatcher resultReadyAt" << index;
    });
    connect(m_pWatcher, &QFutureWatcher<void>::progressRangeChanged, [=](int minimum, int maximum){
        qDebug() << "QFutureWatcher progressRangeChanged" << minimum << maximum;
    });
    connect(m_pWatcher, &QFutureWatcher<void>::progressValueChanged, [=](int value){
        qDebug() << "QFutureWatcher progressValueChanged" << value;
    });
    connect(m_pWatcher, &QFutureWatcher<void>::finished, [=](){
        qDebug() << "QFutureWatcher finished";
    });
}