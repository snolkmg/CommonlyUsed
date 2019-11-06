#ifndef COMMONS_H
#define COMMONS_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

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
        return md5;
        file.flush();
        file.close();
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
        qDebug() << "顺序列表：" << list;
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

    static QString unescapeHtml(QString arg){
        QByteArray array(u8R"({"&AElig;":"Æ","&Aacute;":"Á","&Agrave;":"À","&Atilde;":"Ã","&Auml":"Ä","&Ccedil;":"Ç","&ETH;":"Ð","&Eacute;":"É","&Ecirc;":"Ê","&Egrave;":"È","&Euml;":"Ë","&Iacute;":"Í","&Icirc;":"Î","&Igrave;":"Ì","&Iuml;":"Ï","&Ntilde;":"Ñ","&Oacute;":"Ó","&Ocirc;":"Ô","&Ograve;":"Ò","&Oslash;":"Ø","&Otilde;":"Õ","&Ouml;":"Ö","&THORN;":"Þ","&Uacute;":"Ú","&Ucirc;":"Û","&Ugrave;":"Ù","&Uuml;":"Ü","&Yacute;":"Ý","&aacute;":"á","&acirc;":"â","&acute;":"′","&aelig;":"æ","&agrave;":"à","&aring;":"å","&atilde;":"ã","&auml;":"ä","&brvbar;":"|","&ccedil;":"ç","&cedil;":"?","&cent;":"￠","&circ;":"Â","&copy;":"©","&curren;":"¤","&deg;":"°","&divide;":"÷","&eacute;":"é","&ecirc;":"ê","&egrave;":"è","&euml;":"ë","&frac12;":"?","&frac14;":"?","&frac34;":"?","&iacute;":"í","&icirc;":"î","&ieth;":"ð","&iexcl;":"?","&igrave;":"ì","&iquest;":"?","&iuml;":"ï","&laquo;":"?","&macr;":"ˉ","&micro;":"μ","&middot;":"·","&not;":"?","&ntilde;":"ñ","&oacute;":"ó","&ocirc;":"ô","&ograve;":"ò","&ordf;":"a","&ordm;":"o","&oslash;":"ø","&otilde;":"õ","&ouml;":"ö","&para;":"?","&plusmn;":"±","&pound;":"￡","&raquo;":"?","&reg;":"®","&ring;":"Å","&sect;":"§","&shy;":"/x7f","&sup1;":"1","&sup2;":"2","&sup3;":"3","&szlig;":"ß","&thorn;":"þ","&times;":"&amp;times;","&uacute;":"ú","&ucirc;":"û","&ugrave;":"ù","&uml;":"¨","&uuml;":"ü","&yacute;":"ý","&yen;":"￥","&yuml;":"ÿ","&quot;":"\"","&lt;":"<","&gt;":">","&nbsp;":" ","&amp;":"&"})");
        auto json = QJsonDocument::fromJson(array);
        auto root = json.object();
        for(auto i=root.begin();i!=root.end();i++)
            arg.replace(i.key(),i.value().toString());
        return arg;
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

    static QString readIni()
    {
        QSettings settings;
        QString name = settings.value("verify/name").toString();
        QString pwd = settings.value("verify/pwd").toString();
        qDebug() << "user:" << name << pwd;
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
};

#endif // COMMONS_H
