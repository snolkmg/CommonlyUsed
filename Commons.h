#ifndef COMMONS_H
#define COMMONS_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

static QStringList cnList1 = QStringList() << QStringLiteral("零") << QStringLiteral("一") << QStringLiteral("二")
                                   << QStringLiteral("三") << QStringLiteral("四") << QStringLiteral("五")
                                   << QStringLiteral("六") << QStringLiteral("七") << QStringLiteral("八")
                                   << QStringLiteral("九");
static QList<qint64> numList1 = QList<qint64>() << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;

static QStringList cnList2 = QStringList() << QStringLiteral("十") << QStringLiteral("百") << QStringLiteral("千")
                                    << QStringLiteral("万") << QStringLiteral("亿") << QStringLiteral("兆");
static QList<qint64> numList2 = QList<qint64>() << 10 << 100 << 1000 << 10000 << 100000000 << 1000000000000;

static QStringList circleNumberList = QStringList()
        << "①" << "②" << "③" << "④" << "⑤" << "⑥" << "⑦" << "⑧" << "⑨" << "⑩"
        << "⑪" << "⑫" << "⑬" << "⑭" << "⑮" << "⑯" << "⑰" << "⑱" << "⑲" << "⑳"
        << "㉑" << "㉒" << "㉓" << "㉔" << "㉕" << "㉖" << "㉗" << "㉘" << "㉙" << "㉚"
        << "㉛" << "㉜" << "㉝" << "㉞" << "㉟" << "㊱" << "㊲" << "㊳" << "㊴" << "㊵"
        << "㊶" << "㊷" << "㊸" << "㊹" << "㊺" << "㊻" << "㊼" << "㊽" << "㊾" << "㊿";

static QStringList withinFiftyList = QStringList()
        << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10"
        << "11" << "12" << "13" << "14" << "15" << "16" << "17" << "18" << "19" << "20"
        << "21" << "22" << "23" << "24" << "25" << "26" << "27" << "28" << "29" << "30"
        << "31" << "32" << "33" << "34" << "35" << "36" << "37" << "38" << "39" << "40"
        << "41" << "42" << "43" << "44" << "45" << "46" << "47" << "48" << "49" << "50";

class Commons : public QObject
{
    Q_OBJECT
public:
    explicit Commons(QObject *parent = 0) : QObject(parent){}

    static QString SQL_PATH;
    static QString WORK_PATH;
    static QString DESKTOP_PATH;

    static void writeFile(QString filePath, QString allText, bool isAppend = false) {
        QFile file(filePath);
        if(isAppend)
            file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);
        else
            file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream in(&file);
        in.setCodec("utf-8");
        in << allText;
        file.close();
    }

    static void writeFileBa(QString filePath, QByteArray bytes, bool isAppend = false) {
        QFile file(filePath);
        if(isAppend)
            file.open(QIODevice::WriteOnly | QIODevice::Append);
        else
            file.open(QIODevice::WriteOnly);
        file.write(bytes);
        file.close();
    }

    static QString readFile(QString filePath) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString allText = QString(file.readAll());
        file.close();
        return allText;
    }

    static QByteArray readFileBa(QString filePath) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray ba = file.readAll();
        file.close();
        return ba;
    }

    static QString regMatch(QRegularExpression reg, QString text, int i = 1) {
        QString str;
        QRegularExpressionMatch match = reg.match(text);
        if(match.hasMatch())
            str = match.captured(i);
        return str;
    }

    static QStringList regGlobalMatch(QRegularExpression reg, QString text,
                                      QString filter = QString(), int k = 1) {
        QStringList list;
        QRegularExpressionMatchIterator i = reg.globalMatch(text);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            if(match.hasMatch()) {
                QString cap = match.captured(k);
                if(filter.isEmpty())
                    list.append(cap);
                else if(!cap.contains(filter))
                    list.append(cap);
            }
        }
        return list;
    }

    //获取目标路径下所有文件（含过滤）
    static QStringList getAllFiles(QString dirPath,
                                   QStringList filters = QStringList(),
                                   QStringList list = QStringList())
    {
        QDir dir(dirPath);
        QFileInfoList info_list = dir.entryInfoList(filters, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                                    | QDir::NoSymLinks | QDir::AllDirs);
        foreach(QFileInfo file_info, info_list) {
            if(file_info.isDir())
                list = getAllFiles(file_info.absoluteFilePath(), filters, list);
            else if(file_info.isFile())
                list << file_info.absoluteFilePath();
        }
        return list;
    }

    //获取目标路径下所有文件名（含过滤）
    static QStringList getAllFileNames(QString dirPath,
                                   QStringList filters = QStringList(),
                                   QStringList list = QStringList())
    {
        QDir dir(dirPath);
        QFileInfoList info_list = dir.entryInfoList(filters, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                                    | QDir::NoSymLinks | QDir::AllDirs);
        foreach(QFileInfo file_info, info_list) {
            if(file_info.isDir())
                list = getAllFileNames(file_info.absoluteFilePath(), filters, list);
            else if(file_info.isFile())
                list << file_info.fileName();
        }
        return list;
    }

    //获取目标路径下所有文件baseName（含过滤）
    static QStringList getAllBaseNames(QString dirPath,
                                   QStringList filters = QStringList(),
                                   QStringList list = QStringList())
    {
        QDir dir(dirPath);
        QFileInfoList info_list = dir.entryInfoList(filters, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                                    | QDir::NoSymLinks | QDir::AllDirs);
        foreach(QFileInfo file_info, info_list) {
            if(file_info.isDir())
                list = getAllBaseNames(file_info.absoluteFilePath(), filters, list);
            else if(file_info.isFile())
                list << file_info.baseName();
        }
        return list;
    }

    //获取当前路径下文件（含过滤）
    static QStringList getCurrentFiles(QString dirPath, QStringList filters = QStringList())
    {
        QStringList list;
        QDir dir(dirPath);
        QFileInfoList info_list = dir.entryInfoList(filters, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                                    | QDir::NoSymLinks | QDir::AllDirs);
        foreach(QFileInfo file_info, info_list)
            list << file_info.absoluteFilePath();
        return list;
    }

    //获取目标路径下所有目录
    static QStringList getAllDirs(QString dirPath, QStringList list = QStringList())
    {
        list.append(dirPath);
        QDir dir(dirPath);
        QFileInfoList info_list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::AllDirs);
        foreach(QFileInfo file_info, info_list) {
            if(file_info.isDir())
                list = getAllDirs(file_info.absoluteFilePath(), list);
        }
        return list;
    }

    //获取随机数
    static int randomInt(int min, int max) {
        Q_ASSERT(min < max);
        // 加入随机种子。种子是当前时间距离0点0分0秒的秒数。
        // 每次启动程序，只添加一次种子，以做到数字真正随机。
        static bool seedStatus;
        if (!seedStatus)
        {
            qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
            seedStatus = true;
        }
        int nRandom = qrand() % (max - min);
        nRandom = min + nRandom;

        return nRandom;
    }

    //字节转KB、MB、GB
    static QString byteUnit(qint64 bytes)
    {
        QString strUnit;
        double dSize = bytes * 1.0;
        if (dSize <= 0)
        {
            dSize = 0.0;
        }
        else if (dSize < 1024)
        {
            strUnit = "Bytes";
        }
        else if (dSize < 1024 * 1024)
        {
            dSize /= 1024;
            strUnit = "KB";
        }
        else if (dSize < 1024 * 1024 * 1024)
        {
            dSize /= (1024 * 1024);
            strUnit = "MB";
        }
        else
        {
            dSize /= (1024 * 1024 * 1024);
            strUnit = "GB";
        }

        return QString("%1 %2").arg(QString::number(dSize, 'f', 2)).arg(strUnit);
    }

    //速度转KB/秒、MB/秒、GB/秒
    static QString speedStr(qint64 bytes)
    {
        double speed = bytes * 1.0;
        QString strUnit;
        if (speed <= 0)
        {
            speed = 0;
            strUnit = "Bytes/秒";
        }
        else if (speed < 1024)
        {
            strUnit = "Bytes/秒";
        }
        else if (speed < 1024 * 1024)
        {
            speed /= 1024;
            strUnit = "KB/秒";
        }
        else if (speed < 1024 * 1024 * 1024)
        {
            speed /= (1024 * 1024);
            strUnit = "MB/秒";
        }
        else
        {
            speed /= (1024 * 1024 * 1024);
            strUnit = "GB/秒";
        }

        QString strSpeed = QString::number(speed, 'f', 2);
        return QString("%1 %2").arg(strSpeed).arg(strUnit);
    }

    // 秒转*d *h *m *s
    static QString timeFormat(int seconds)
    {
        QString strValue;
        QString strSpacing(" ");
        if (seconds <= 0)
        {
            strValue = QString("%1 秒").arg(0);
        }
        else if (seconds < 60)
        {
            strValue = QString("%1 秒").arg(seconds);
        }
        else if (seconds < 60 * 60)
        {
            int nMinute = seconds / 60;
            int nSecond = seconds - nMinute * 60;

            strValue = QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        }
        else if (seconds < 60 * 60 * 24)
        {
            int nHour = seconds / (60 * 60);
            int nMinute = (seconds - nHour * 60 * 60) / 60;
            int nSecond = seconds - nHour * 60 * 60 - nMinute * 60;

            strValue = QString("%1 小时").arg(nHour);

            if (nMinute > 0)
                strValue += strSpacing + QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        }
        else
        {
            int nDay = seconds / (60 * 60 * 24);
            int nHour = (seconds - nDay * 60 * 60 * 24) / (60 * 60);
            int nMinute = (seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60) / 60;
            int nSecond = seconds - nDay * 60 * 60 * 24 - nHour * 60 * 60 - nMinute * 60;

            strValue = QString("%1 天").arg(nDay);

            if (nHour > 0)
                strValue += strSpacing + QString("%1 小时").arg(nHour);

            if (nMinute > 0)
                strValue += strSpacing + QString("%1 分").arg(nMinute);

            if (nSecond > 0)
                strValue += strSpacing + QString("%1 秒").arg(nSecond);
        }

        return strValue;
    }

    //删除空文件夹
    static void removeEmptyDir(QString dirPath) {
        QDir dir(dirPath);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                               | QDir::NoSymLinks | QDir::AllDirs);

        if(list.isEmpty()) {
            dir.removeRecursively();
            return;
        }
        foreach (QFileInfo info, list) {
            if(info.isDir())
                removeEmptyDir(info.absoluteFilePath());
        }
    }

    //任意网页编码转utf-8编码
    static QByteArray codecToUTF8(QByteArray bytes) {
        QTextCodec *codec = QTextCodec::codecForHtml(bytes);
        QString unicode = codec->toUnicode(bytes);
        QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
        QByteArray ba = utf8->fromUnicode(unicode);
        return ba;
    }

    //两种编码互转
    static QByteArray codec1ToCodec2(QByteArray bytes, QByteArray codec1, QByteArray codec2 = "UTF-8") {
        QTextCodec * c1 = QTextCodec::codecForName(codec1);
        QTextCodec *c2 = QTextCodec::codecForName(codec2);
        QString unicode = c1->toUnicode(bytes);
        QByteArray ba = c2->fromUnicode(unicode);
        return ba;
    }

    //获取文件md5
    static QString getMd5(QString filePath) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        QCryptographicHash md5Hash(QCryptographicHash::Md5);
        while(!file.atEnd()) {
            QByteArray ba = file.read(8192);
            md5Hash.addData(ba);
        }
        QString md5 = md5Hash.result().toHex().toUpper();
        file.flush();
        file.close();
        return md5;
    }

    //合法的标题
    static QString legalTitle(QString title) {
        QRegularExpression reg("[\\\\/\\*\"<>|]");
        title = title.replace(reg, "").replace(":", "：").replace("?", "？");
        return title;
    }

    //转义1（待续）
    static QString textEscape(QString text) {
        text = text.replace(".", QString("\\."))
                .replace("+", QString("\\+"))
                .replace("-", QString("\\-"));
        return text;
    }

    //转义2（待续）
    static QStringList listEscape(QStringList list) {
        QStringList list2;
        foreach (QString text, list)
            list2.append(textEscape(text));
        return list2;
    }

    //字符串按照拼音排序
    static QStringList pinYinList(QStringList list) {
        QLocale cn(QLocale::Chinese);
        QCollator collator(cn);
        qStableSort(list.begin(), list.end(), collator);
//        qDebug() << "顺序列表：" << list;
        return list;
    }

    //列表逆序
    static QStringList listReverse(QStringList list) {
        QStringList reverseList;
        QListIterator<QString> it(list);
        while (it.hasNext())
            reverseList.push_front(it.next());
        return reverseList;
    }

    //删除当前文件夹
    static bool removeCurrentDir(QDir dir) {
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                      | QDir::NoSymLinks | QDir::AllDirs);
        return dir.removeRecursively();
    }

    //删除当前文件夹2
    static bool removeCurrentDir(QString dirPath) {
        QDir dir(dirPath);
        return removeCurrentDir(dir);
    }

    static bool removeRecursiveDir(QString dirPath) {
        QDir dir(dirPath);
        QFileInfoList info_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot
                                                    | QDir::NoSymLinks | QDir::AllDirs);
        foreach(QFileInfo file_info, info_list) {
            if (file_info.isDir())
                removeRecursiveDir(file_info.absoluteFilePath());
            else if (file_info.isFile()) {
                if(!QFile::remove(file_info.absoluteFilePath()))
                    return false;
            }
        }
        return dir.removeRecursively();
    }

    //数字转字符串补零
    static QString numFormat(int k, int count) {
        QString str = QString::number(count);
        int size = str.size();
        return QString("%1").arg(k, size, 10, QLatin1Char('0'));
    }

    //1.图片转base64
    static QByteArray imageToBase64(const QString &ImgPath)//ImgPath: 图片路径
    {
        QImage image(ImgPath);
        QByteArray ba;
        QBuffer buf(&ba);
        image.save(&buf);//图片格式
        QByteArray hexed = ba.toBase64();
        buf.close();
        return hexed;
    }

    //2.base64转图片
    static QImage base64ToImage(const QByteArray &base)//base: base64值
    {
        QByteArray realBa = QByteArray::fromBase64(base);
        QBuffer buffer(&realBa);
        buffer.open(QIODevice::WriteOnly);
        QImage image;
        image.loadFromData(realBa);
        buffer.close();
        return image;
    }

    static void genFileIcon(QString suffix, int size)
    {
        QFileIconProvider provider;
        QIcon icon;

        QDir iconDir = QDir(QDir::current().absoluteFilePath("icons"));
        if(!iconDir.exists())
            QDir::current().mkdir("icons");

        QString strTemplateName = iconDir.absolutePath() + "/" +
                QCoreApplication::applicationName() + "_XXXXXX." + suffix;
        qDebug() << "临时路径：" << strTemplateName;
        QTemporaryFile tmpFile(strTemplateName);
    //    tmpFile.setAutoRemove(false);
        QString file_name;
        if (tmpFile.open()) {
            file_name = tmpFile.fileName();
            qDebug() << "临时文件：" << file_name;
            icon = provider.icon(QFileInfo(file_name));
            tmpFile.close();
            QPixmap pixmap = icon.pixmap(size, size);
            QImage img = pixmap.toImage();
            img.save(iconDir.absoluteFilePath(QString("%1.png").arg(suffix)), "PNG", 100);
        }
        else
            qCritical() << QString("生成图标失败 %1").arg(tmpFile.fileName());
    }

    static QStringList iconList()
    {
        QDir iconDir = QDir(QDir::current().absoluteFilePath("icons"));
        return iconDir.entryList(QStringList(QString("*.png")), QDir::Files);
    }

    static QIcon fileIcon(QString suffix, int size = 256) {
        QFileIconProvider icon_provider;
        QIcon icon;
        if(!suffix.isEmpty()) {
            QStringList iList = iconList();
            if(!iList.contains(QString("%1.png").arg(suffix)))
                genFileIcon(suffix, size);
            icon = QIcon(QString(QDir(QDir::current().absoluteFilePath("icons"))
                                 .absoluteFilePath(suffix)));
        } else {
            icon = icon_provider.icon(QFileIconProvider::File);
        }
        return icon;
    }

    static void setLabelWidth(QLabel *label, int limitWidth, Qt::TextElideMode mode = Qt::ElideRight)
    {
        QFontMetrics fontMetrics(label->font());
        int fontSize = fontMetrics.width(label->text());
        QString str = label->text();
        if(fontSize > limitWidth)
            str = fontMetrics.elidedText(label->text(), mode, limitWidth);
        label->setText(str);
    }

    static QString workPath()
    {
        QDir serverDir = QDir(QDir::current().absoluteFilePath("server"));
        QString path = serverDir.absoluteFilePath("ip.txt");
        return readFile(path).trimmed();
    }

    static QString readIni()
    {
        QSettings settings;
//        QString name = settings.value("verify/name").toString();
//        QString pwd = settings.value("verify/pwd").toString();
//        qDebug() << "user:" << name << pwd;
        QString authorization = settings.value("verify/authorization").toString();
        return authorization;
    }

    static QString readDownloadPath()
    {
        QSettings settings;
        QString downloadpath = settings.value("verify/downloadpath").toString();
        return downloadpath;
    }

    static QStringList readUserIni()
    {
        QSettings settings;
        QString name = settings.value("verify/name").toString();
        QString pwd = settings.value("verify/pwd").toString();
        QString authorization = settings.value("verify/authorization").toString();
        QString downloadpath = settings.value("verify/downloadpath").toString();
        QStringList list;
        list << name << pwd << authorization << downloadpath;
        return list;
    }

    static void addSetting(QString path)
    {
        QSettings settings;
        settings.beginGroup(QString("verify"));
        settings.setValue(QString("downloadpath"), path);
        settings.endGroup();
    }

    static void resetPassword(QString password)
    {
        QSettings settings;
        settings.beginGroup(QString("verify"));
        settings.setValue(QString("pwd"), password);
        settings.endGroup();
    }

    //获取excel列（最多702列【ZZ列】）
    static QString excelNumberToLetter(int k) {
        if(k > 702)
            return "";
        int m, n = 0;
        m = k / 26;
        n = k % 26;
        if(m == 0) {
            return QString(QChar(k + 64));
        } else {
            if(n == 0) {
                if(m == 1)
                    return QString(QChar(k + 64));
                else
                    return QString(QChar(m + 63)) + QString(QChar(90));
            } else
                return QString(QChar(m + 64)) + QString(QChar(n + 64));
        }
    }

    static QString dateFormat(QString dateStr) {
        QString cnStr = dateStr;
        QDate pubDate = QDate::fromString(cnStr, "yyyy-MM-dd");
        if(pubDate.isNull())
            pubDate = QDate::fromString(cnStr, "yyyy-M-d");
        if(pubDate.isNull())
            pubDate = QDate::fromString(cnStr, "yyyy-MM");
        if(pubDate.isNull())
            pubDate = QDate::fromString(cnStr, "yyyy-M");
        if(pubDate.isNull())
            pubDate = QDate::fromString(cnStr, "yyyy/MM/dd");
        if(pubDate.isNull())
            pubDate = QDate::fromString(cnStr, "yyyy/M/d");
        if(pubDate.isNull())
            pubDate = QDate::fromString(cnStr, "yyyy/MM");
        if(pubDate.isNull())
            pubDate = QDate::fromString(cnStr, "yyyy/M");
        if(pubDate.isNull())
            pubDate = QDate::fromString(cnStr, "yyyyMMdd");
        if(pubDate.isNull())
            return dateStr;
        qDebug() << pubDate;
        return pubDate.toString("yyyy-MM-dd");
    }

    //QStringList 去重
    //!使用样例：removeListSame(&list);
    static void removeListSame(QStringList *list) {
        for (int i = 0; i < list->count(); i++) {
            for (int k = i + 1; k <  list->count(); k++) {
                if (list->at(i) ==  list->at(k)) {
                    list->removeAt(k);
                    k--;
                }
            }
        }
    }

    //QStringList 查找重复项
    //!使用样例：findListSame(list);
    static QList<int> findListSame(QStringList list, bool isAscending = false) {
        QList<int> list1;
        for (int i = 0; i < list.count(); i++) {
            for (int k = i + 1; k <  list.count(); k++) {
                if (list.at(i) == list.at(k)) {
                    if(!list1.contains(k))
                        list1.append(k);
                }
            }
        }
        if(isAscending)
            qStableSort(list1.begin(), list1.end(), qLess<int>());
        else
            qStableSort(list1.begin(), list1.end(), qGreater<int>());
        return list1;
    }

    static QStringList removeDuplicatesByIndex(QList<int> intList, QStringList list) {
        for(int i = 0; i < intList.size(); i++) {
            int k = intList.at(i);
            list.removeAt(k);
        }
        return list;
    }

    static QStringList listRemoveDuplicates(QStringList list, QStringList codeList) {
        QList<int> intList = findListSame(codeList);
//        qDebug() << intList;
        removeDuplicatesByIndex(intList, list);
        return list;
    }

    //自定义文件特征码1：文件前8192个字节md5
    static QString fileFeatureCode(QString filePath) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        QCryptographicHash md5Hash(QCryptographicHash::Md5);
        QByteArray ba = file.read(8192);
        md5Hash.addData(ba);
        QString md5 = md5Hash.result().toHex();
        file.flush();
        file.close();
        return md5;
    }

    //自定义文件特征码2：文件前8192个字节md5+文件大小
    static QString fileFeatureCodeWithSize(QString filePath) {
        QFile file(filePath);
        file.open(QIODevice::ReadOnly);
        QCryptographicHash md5Hash(QCryptographicHash::Md5);
        QByteArray ba = file.read(8192);
        md5Hash.addData(ba);
        QString md5 = md5Hash.result().toHex();
        QString fileSize = QString::number(file.size());
        file.flush();
        file.close();
        return QString("%1_%2").arg(md5).arg(fileSize);
    }

    static bool openFileAndSelect(QString filePath) {
        filePath.replace("/", "\\");
        QString cmd = QString("explorer.exe /select,%1").arg(filePath);
        return QProcess::startDetached(cmd);
    }

    //模板逆向
    template<class T>
    static T strReversed(T str) {
        T str2;
        for(auto i : str)
            str2.prepend(i);
        return str2;
    }

    //中文数字转阿拉伯数字
    static qint64 cnToNum(QString str) {
        QList<qint64> nums;
        qint64 unit = 1;
        QString text = strReversed(str);
        qDebug() << text;
        for(QString s : text) {
            if(cnList1.contains(s)) {
                int k = cnList1.indexOf(s);
                qint64 num = numList1.at(k) * unit;
                nums.append(num);
            } else {
                int k = cnList2.indexOf(s);
                qint64 value = numList2.at(k);
                if(value >= 10000 && value <=100000000) {
                    nums.append(value);
                    unit = 1;
                } else {
                    unit = value;
                }
            }
        }

        qDebug() << nums << unit;
        if(unit == 10 && str.startsWith(cnList2.at(0)))
            nums.append(10);
        qDebug() << nums << unit;

        qint64 tmp = 0;
        qint64 res = 0;
        QList<qint64> nums2 = strReversed(nums);
        qDebug() << nums2;
        for(qint64 num : nums2) {
            if(num >= 10000 && num <=100000000) {
                tmp *= num;
                res += tmp;
                tmp = 0;
            } else {
                tmp += num;
            }
        }
        res += tmp;
        return res;
    }

    //阿拉伯数字转中文数字
    static QString numToCn(QString numStr) {
        QRegularExpression reg("(\\d+)");
        QString str = regMatch(reg, numStr);
        if(str.isEmpty())
            return QString("数字有误");
        qint64 num = str.toLongLong();
        if(num == 0)
            return QString("数字过大");
        QString text;
        int k = 0;
        qint64 size = num;
        while (size /= 10000) {
            k++;
        }
        QString temp = str;
        QStringList list;
        while (temp.size() > 4) {
            QString temp1 = temp.mid(temp.size() - 4);
            QString temp2 = temp.mid(0, temp.size() - 4);
            temp = temp2;
            list << temp1;
        }
        list << temp;
        int count = list.size();
        QString spe("0000");
        if(count > 5)
            return QString("数字有误");
        text = list.at(0) == spe ? QString("零") : tenThousandToCn(list.at(0));
        if(count > 1)
            text = list.at(1) == spe ? QString("零%1").arg(text)
                                     : QString("%1万%2").arg(tenThousandToCn(list.at(1))).arg(text);
        if(count > 2)
            text = list.at(2) == spe ? QString("零%1").arg(text)
                                     : QString("%1亿%2").arg(tenThousandToCn(list.at(2))).arg(text);

        if(count > 3)
            text = list.at(3) == spe ? QString("零%1").arg(text)
                                     : QString("%1兆%2").arg(tenThousandToCn(list.at(3))).arg(text);
        if(count > 4)
            text = list.at(4) == spe ? QString("零%1").arg(text)
                                     : QString("%1京%2").arg(tenThousandToCn(list.at(4))).arg(text);
        QRegularExpression reg1("(零)+$", QRegularExpression::MultilineOption);
        QRegularExpression reg2("(零)+");
        QRegularExpression reg3("^(一十)(.*?)$", QRegularExpression::MultilineOption);
        text.replace(reg1, "").replace(reg2, QString("零")).replace(reg3, QString("十\\2"));
        return text;
    }

    //阿拉伯数字转中文数字（一万以内）
    static QString tenThousandToCn(QString numStr) {
        int num = numStr.toInt();
        if(num == 0)
            return QString("零");
        QString str = QString::number(num);
        QString text;
        QList<int> list;
        for(int i = 0; i < str.size(); i++)
            list.prepend(QString(str.at(i)).toInt());
        text = cnList1.at(numList1.indexOf(list.at(0)));
        if(list.size() > 1) {
            QString temp = list.at(1) == 0 ? QString("零")
                                           : cnList1.at(numList1.indexOf(list.at(1))) + QString("十");
            text = text.prepend(temp);
        }
        if(list.size() > 2) {
            QString temp = list.at(2) == 0 ? QString("零")
                                           : cnList1.at(numList1.indexOf(list.at(2))) + QString("百");
            text = text.prepend(temp);
        }
        if(list.size() > 3) {
            QString temp = cnList1.at(numList1.indexOf(list.at(3))) + QString("千");
            text = text.prepend(temp);
        }
        QRegularExpression reg("(零)+$", QRegularExpression::MultilineOption);
        text.replace(reg, "");
        if(numStr.startsWith("0"))
            text = text.prepend("零");
        return text;
    }

    //查找乱码
    static QStringList garbledList(QString text) {
        QRegularExpression reg("([^\\x00-\\xff\\w`~!@#\\$%\\^&\\*\\(\\)_\\-\\+=<>?:\"{}|,.\\\\/;'\\[\\]·~！￥……（）——《》？：“”【】、；‘’，。、「」])",
                               QRegularExpression::MultilineOption | QRegularExpression::UseUnicodePropertiesOption);
//        qDebug() << reg;
        QStringList list = Commons::regGlobalMatch(reg, text);
        list.removeDuplicates();
        return list;
    }

    //列表平均分为n个子列表
    template<class T>
    static QList< QList<T> > subTList(QList<T> list, int n) {
        QList< QList<T> > totalList;
        if(list.size() <= n || n < 1) {
            totalList << list;
        } else {
            while (list.size() > n) {
                QList<T> subList = list.mid(0, n);
                list = list.mid(n);
                totalList << subList;
            }
            if(list.size() > 0)
                totalList << list;
        }
        return totalList;
    }

    //Qt 数字转字符串，千分位分隔符
    static QString numberToStr(int k) {
        QString str = QString("%L1").arg(k);
        return str;
    }

    //Qt 浮点数转字符串，千分位分隔符
    static QString numberToStr(double k) {
        QString str = QString("%L1").arg(k, 0, 'f', 0);
        return str;
    }

    static QStringList initList(int k, QString arg = QString()) {
        if(k < 1)
            k = 1;
        QStringList list;
        for(int i = 0; i < k; i++)
            list << arg;
        return list;
    }

    //圈数字转普通数字
    static QString circleToNumber(QString text, QString prefix = QString(), QString suffix = QString()) {
        foreach (QString circleNum, circleNumberList) {
            int index = circleNumberList.indexOf(circleNum);
            if(text.contains(circleNum))
                text.replace(circleNum, QString("%1%2%3").arg(prefix).arg(withinFiftyList.at(index)).arg(suffix));
        }
        return text;
    }

    //Qt图片调整之亮度调节
    static QImage adjustBrightness(QImage Img, int iBrightValue)
    {
        int red, green, blue;
        int pixels = Img.width() * Img.height();
        unsigned int *data = (unsigned int *)Img.bits();

        for (int i = 0; i < pixels; ++i)
        {
            red= qRed(data[i])+ iBrightValue;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green= qGreen(data[i]) + iBrightValue;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue= qBlue(data[i]) + iBrightValue;
            blue =  (blue  < 0x00) ? 0x00 : (blue  > 0xff) ? 0xff : blue ;
            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }

        return Img;
    }

    //Qt图片调整之对比度调节
    static QImage adjustContrast(QImage Img, int iContrastValue)
    {
        int pixels = Img.width() * Img.height();
        unsigned int *data = (unsigned int *)Img.bits();

        int red, green, blue, nRed, nGreen, nBlue;

        if (iContrastValue > 0 && iContrastValue < 100)
        {
            float param = 1 / (1 - iContrastValue / 100.0) - 1;

            for (int i = 0; i < pixels; ++i)
            {
                nRed = qRed(data[i]);
                nGreen = qGreen(data[i]);
                nBlue = qBlue(data[i]);

                red = nRed + (nRed - 127) * param;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = nGreen + (nGreen - 127) * param;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nBlue + (nBlue - 127) * param;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                data[i] = qRgba(red, green, blue, qAlpha(data[i]));
            }
        }
        else
        {
            for (int i = 0; i < pixels; ++i)
            {
                nRed = qRed(data[i]);
                nGreen = qGreen(data[i]);
                nBlue = qBlue(data[i]);

                red = nRed + (nRed - 127) * iContrastValue / 100.0;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = nGreen + (nGreen - 127) * iContrastValue / 100.0;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nBlue + (nBlue - 127) * iContrastValue / 100.0;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                data[i] = qRgba(red, green, blue, qAlpha(data[i]));
            }
        }

        return Img;
    }

    //Qt图片调整之饱和度调节
    static QImage adjustSaturation(QImage Img, int iSaturateValue)
    {
        int red, green, blue, nRed, nGreen, nBlue;
        int pixels = Img.width() * Img.height();
        unsigned int *data = (unsigned int *)Img.bits();

        float Increment = iSaturateValue/100.0;

        float delta = 0;
        float minVal, maxVal;
        float L, S;
        float alpha;

        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            minVal = std::min(std::min(nRed, nGreen), nBlue);
            maxVal = std::max(std::max(nRed, nGreen), nBlue);
            delta = (maxVal - minVal) / 255.0;
            L = 0.5*(maxVal + minVal) / 255.0;
            S = std::max(0.5*delta / L, 0.5*delta / (1 - L));

            if (Increment > 0)
            {
                alpha = std::max(S, 1 - Increment);
                alpha = 1.0 / alpha - 1;
                red = nRed + (nRed - L*255.0)*alpha;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = nGreen + (nGreen - L*255.0)*alpha;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nBlue + (nBlue - L*255.0)*alpha;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            }
            else
            {
                alpha = Increment;
                red = L*255.0 + (nRed - L * 255.0)*(1+alpha);
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = L*255.0 + (nGreen - L * 255.0)*(1+alpha);
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = L*255.0 + (nBlue - L * 255.0)*(1+alpha);
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            }

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }

        return Img;
    }
};

#endif // COMMONS_H
