//Qt 常用操作

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

//main.cpp 加载翻译文件

    QTranslator tl;
    tl.load("://zh_CN");
    a.installTranslator(&tl);

    QTranslator tl2;
    tl2.load("://qt_zh_CN");
    a.installTranslator(&tl2);

//选择文件
private Q_SLOTS:
    void onSelect();

private:
    QString lastPath;
    void loadFile(const QString &filePath);
    void loadFiles(const QStringList &fileList);
	
	void writeText(QString text);
    void writeMsg(QString text, int msecs = 10000);
    void timeConsuming(int msecs);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

setAcceptDrops(true);
connect(ui->selectBtn, &QPushButton::clicked, this, &readEpub::onSelect);
connect(ui->selectAct, &QAction::triggered, this, &batReadEpub::onSelect);
setStyleSheet(QString("font-family: Microsoft Yahei;"));
statusBar()->setStyleSheet(QString("color: blue; font-size: 14px;"));

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

void readFile::loadFile(const QString &filePath)
{
	//操作单文件……
}

void readFile::loadFiles(const QStringList &fileList)
{
	//操作多文件……
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

//拖放多文件
void readFile::dropEvent(QDropEvent *event)
{
    const QMimeData *mime = event->mimeData();
	QList<QUrl> duplicates;
    QStringList fileList;

    foreach (QUrl url, mime->urls())
    {
        if (duplicates.contains(url))
            continue;
        else
            duplicates << url;
        QString fileName(url.toLocalFile());
        QFileInfo fi(fileName);
        if(fi.isFile())
            fileList << fi.absoluteFilePath();
    }

    //qStableSort(fileList.begin(), fileList.end());
    qDebug() << "拖放文件列表：" << fileList;
    fileList = Commons::pinYinList(fileList);

    if(fileList.isEmpty())
        return;
    loadFiles(fileList);
}

void tagScreenShot::writeText(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}

void tagScreenShot::writeMsg(QString text, int msecs)
{
    ui->plainTextEdit->appendPlainText(text);
    msecs > 0 ? statusBar()->showMessage(text, msecs) : statusBar()->showMessage(text);
}

void tagScreenShot::timeConsuming(int msecs)
{
//    QTime timeUp;
//    timeUp.start();
    double secs = msecs / 1000.0;
    QString timeStr = secs < 60 ? QString("%1 秒").arg(secs) : Commons::timeFormat((int)secs);
    writeText(QString("共计耗时 %1\n").arg(timeStr));
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

    connect(ui->tableWidget, &myTableWidget::itemSender, this, &itemReceiver);
    connect(ui->tableWidget, &myTableWidget::itemDoubleClicked, this, &onOPenFile);
}

void createContextMenu()
{
    popMenu = new QMenu(tr("右键菜单"),  this);
    copyTextAct = new QAction(tr("复制文本"), this);;
    copyPathAct = new QAction(tr("复制文件路径"), this);
    openFileAct = new QAction(tr("打开文件"), this);
    openFolderAct = new QAction(tr("打开所在文件夹"), this);

    connect(copyTextAct, &QAction::triggered, this, &onCopyText);
    connect(copyPathAct, &QAction::triggered, this, &onCopyPath);
    connect(openFileAct, &QAction::triggered, this, &onOPenFile);
    connect(openFolderAct, &QAction::triggered, this, &onOpenFolder);
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
void onCopyText()
{
    QString text = ui->tableWidget->currentItemText();
    if(!text.isEmpty())
        statusBar()->showMessage(tr("“%1”已复制到粘贴板").arg(text), 10000);
}

//复制当前文件路径
void onCopyPath()
{
    QString text = ui->tableWidget->columnItemText(1);
    if(!text.isEmpty())
        statusBar()->showMessage(tr("“%1”已复制到粘贴板").arg(text), 10000);
}

//打开文件
void onOPenFile()
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
void onOpenFolder()
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
    QString cmd = QString("explorer.exe /select,%1").arg(filePath);
    qDebug() << "打开文件：" << cmd;
    proc.startDetached(cmd);
}

void itemReceiver(QTableWidgetItem *item)
{
    QTableWidget *myTable = (QTableWidget *)sender();
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