#ifndef WIDTHFORMS_H
#define WIDTHFORMS_H

#include <QObject>
#include <QPair>
#include <QList>
#include <QChar>

typedef QPair<ushort, ushort> ushortPair;
typedef QList<ushortPair> ushortPairList;

typedef QPair<QChar, QChar> QCharPair;
typedef QList<QCharPair> QCharPairList;

typedef QPair<QString, QString> QStringPair;
typedef QList<QStringPair> QStringPairList;

class WidthForms : public QObject
{
    Q_OBJECT
public:
    explicit WidthForms(QObject *parent = 0);

    //! 全角字符转半角字符
    static QString toHalf(QString text);
    static QString toHalfushort(QString text, ushortPairList list = ushortCommonList());
    static QString toHalfQChar(QString text, QCharPairList list = charCommonList());
    static QString toHalfQString(QString text, QStringPairList list = stringCommonList());
    //! 半角字符转全角字符
    static QString toFull(QString text);
    static QString toFullushort(QString text, ushortPairList list = ushortCommonList());
    static QString toFullQChar(QString text, QCharPairList list = charCommonList());
    static QString toFullQString(QString text, QStringPairList list = stringCommonList());

    //! 过滤（全角字符）列表
    static QList<ushort> ushortFilters(bool fullSpace = false);
    static QList<QChar> charFilters(bool fullSpace = false);
    static QStringList stringFilters(bool fullSpace = false);

    //! ushort 全角半角 对 列表
    static ushortPairList ushortAllList();
    static ushortPairList ushortSpaceList();
    static ushortPairList ushortNumberList();
    static ushortPairList ushortCaseList();
    static ushortPairList ushortUpperCaseList();
    static ushortPairList ushortLowerCaseList();
    static ushortPairList ushortCommonList();
    static ushortPairList ushortCustomList(QList<ushort> filters = ushortFilters());

    //! QChar 全角半角 对 列表
    static QCharPairList charAllList();
    static QCharPairList charSpaceList();
    static QCharPairList charNumberList();
    static QCharPairList charCaseList();
    static QCharPairList charUpperCaseList();
    static QCharPairList charLowerCaseList();
    static QCharPairList charCommonList();
    static QCharPairList charCustomList(QList<QChar> filters = charFilters());

    //! QString 全角半角 对 列表
    static QStringPairList stringAllList();
    static QStringPairList stringSpaceList();
    static QStringPairList stringNumberList();
    static QStringPairList stringCaseList();
    static QStringPairList stringUpperCaseList();
    static QStringPairList stringLowerCaseList();
    static QStringPairList stringCommonList();
    static QStringPairList stringCustomList(QStringList filters = stringFilters());

    enum ConvertMode {
        CommonMode,
        AllMode,
        NumberMode,
        CaseMode,
        UpperCaseMode,
        LowerCaseMode,
        OnlySpaceMode,
        CustomMode
    };

    static QString convertText(QString text, ConvertMode mode, bool isFull = true);

private:
    // 转换
    static QString ushortConvert(QString text, ushortPairList list, bool isFull);
    static QString charConvert(QString text, QCharPairList list, bool isFull);
    static QString stringConvert(QString text, QStringPairList list, bool isFull);
};

#endif // WIDTHFORMS_H
