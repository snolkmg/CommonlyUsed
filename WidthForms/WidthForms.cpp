#include "WidthForms.h"

WidthForms::WidthForms(QObject *parent) :
    QObject(parent)
{

}

//! 全角字符转半角字符（默认常用列表 charCommonList()）
QString WidthForms::toHalf(QString text)
{
    return toHalfQChar(text);
}

//! 全角字符转半角字符（默认常用列表 ushortPairList()）
QString WidthForms::toHalfushort(QString text, ushortPairList list)
{
    return ushortConvert(text, list, true);
}

//! 全角字符转半角字符（默认常用列表 charCommonList()）
QString WidthForms::toHalfQChar(QString text, QCharPairList list)
{
    return charConvert(text, list, true);
}

//! 全角字符转半角字符（默认常用列表 stringCommonList()）
QString WidthForms::toHalfQString(QString text, QStringPairList list)
{
    return stringConvert(text, list, true);
}

//! 半角字符转全角字符（默认常用列表 charCommonList()）
QString WidthForms::toFull(QString text)
{
    return toFullQChar(text);
}

//! 半角字符转全角字符（默认常用列表 ushortPairList()）
QString WidthForms::toFullushort(QString text, ushortPairList list)
{
    return ushortConvert(text, list, false);
}

//! 半角字符转全角字符（默认常用列表 charCommonList()）
QString WidthForms::toFullQChar(QString text, QCharPairList list)
{
    return charConvert(text, list, false);
}

//! 半角字符转全角字符（默认常用列表 stringCommonList()）
QString WidthForms::toFullQString(QString text, QStringPairList list)
{
    return stringConvert(text, list, false);
}

//! ushort 全角半角转换
//! isFull: true: 全角转半角，false: 半角转全角
QString WidthForms::ushortConvert(QString text, ushortPairList list, bool isFull)
{
    for(int i = 0; i < list.size(); i++) {
        ushortPair pair = list.at(i);
        ushort full = pair.first;
        ushort half = pair.second;
        if(isFull)
            text.replace(QChar(full), QChar(half));
        else
            text.replace(QChar(half), QChar(full));
    }
    return text;
}

//! QChar 全角半角转换
//! isFull: true: 全角转半角，false: 半角转全角
QString WidthForms::charConvert(QString text, QCharPairList list, bool isFull)
{
    for(int i = 0; i < list.size(); i++) {
        QCharPair charPair = list.at(i);
        QChar full = charPair.first;
        QChar half = charPair.second;
        if(isFull)
            text.replace(full, half);
        else
            text.replace(half, full);
    }
    return text;
}

//! QString 全角半角转换
//! isFull: true: 全角转半角，false: 半角转全角
QString WidthForms::stringConvert(QString text, QStringPairList list, bool isFull)
{
    for(int i = 0; i < list.size(); i++) {
        QStringPair stringPair = list.at(i);
        QString full = stringPair.first;
        QString half = stringPair.second;
        if(isFull)
            text.replace(full, half);
        else
            text.replace(half, full);
    }
    return text;
}

//! 转换
QString WidthForms::convertText(QString text, ConvertMode mode, bool isFull)
{
    switch (mode) {
    case CommonMode:
        text = charConvert(text, charCommonList(), isFull);
        break;
    case AllMode:
        text = charConvert(text, charAllList(), isFull);
        break;
    case NumberMode:
        text = charConvert(text, charNumberList(), isFull);
        break;
    case CaseMode:
        text = charConvert(text, charCaseList(), isFull);
        break;
    case UpperCaseMode:
        text = charConvert(text, charUpperCaseList(), isFull);
        break;
    case LowerCaseMode:
        text = charConvert(text, charLowerCaseList(), isFull);
        break;
    case OnlySpaceMode:
        text = charConvert(text, charSpaceList(), isFull);
        break;
    case CustomMode:
        text = charConvert(text, charCustomList(), isFull);
        break;
    default:
        break;
    }
    return text;
}

// ushort 所有 全角半角对列表
ushortPairList WidthForms::ushortAllList()
{
    ushortPairList list;
    for(ushort i = 0xFF01; i <= 0xFF5E; i++)
        list << ushortPair(i, i - 0xFEE0);
    list << ushortPair(0x3000, 0x0020);
    return list;
}

// ushort 空格 全角半角对列表
ushortPairList WidthForms::ushortSpaceList()
{
    ushortPairList list;
    list << ushortPair(0x3000, 0x0020);
    return list;
}

// ushort 数字 全角半角对列表
ushortPairList WidthForms::ushortNumberList()
{
    ushortPairList list;
    for(ushort i = 0xFF10; i <= 0xFF19; i++)
        list << ushortPair(i, i - 0xFEE0);
    return list;
}

// ushort 字母 全角半角对列表
ushortPairList WidthForms::ushortCaseList()
{
    ushortPairList list;
    list << ushortUpperCaseList() << ushortLowerCaseList();
    return list;
}

// ushort 字母 大写全角半角对列表
ushortPairList WidthForms::ushortUpperCaseList()
{
    ushortPairList list;
    for(ushort i = 0xFF21; i <= 0xFF3A; i++)
        list << ushortPair(i, i - 0xFEE0);
    return list;
}

// ushort 字母 小写全角半角对列表
ushortPairList WidthForms::ushortLowerCaseList()
{
    ushortPairList list;
    for(ushort i = 0xFF41; i <= 0xFF5A; i++)
        list << ushortPair(i, i - 0xFEE0);
    return list;
}

// ushort 常用 小写全角半角对列表（数字、字母、空格）
ushortPairList WidthForms::ushortCommonList()
{
    ushortPairList list;
    list << ushortNumberList()
         << ushortUpperCaseList()
         << ushortLowerCaseList()
         << ushortSpaceList();
    return list;
}

// ushort 过滤掉的全角列表（无需转换的），fullSpace: 无需转换的列表中是否包括全角空格，默认不包括
QList<ushort> WidthForms::ushortFilters(bool fullSpace)
{
    QList<ushort> filters;
    filters << 0xFF01 << 0xFF08 << 0xFF09 << 0xFF0C << 0xFF1A << 0xFF1B << 0xFF1F;
    //！（），：；？
    if(fullSpace)
        filters << 0x3000;
    return filters;
}

// ushort 自定义 过滤 全角半角对列表（数字、字母、空格）
ushortPairList WidthForms::ushortCustomList(QList<ushort> filters)
{
    ushortPairList list;
    for(ushort i = 0xFF01; i <= 0xFF5E; i++) {
        if(!filters.contains(i))
            list << ushortPair(i, i - 0xFEE0);
    }
    if(!filters.contains(0x3000))
        list << ushortPair(0x3000, 0x0020);
    return list;
}

// QChar 所有 全角半角对列表
QCharPairList WidthForms::charAllList()
{
    QCharPairList list;
    for(ushort i = 0xFF01; i <= 0xFF5E; i++)
        list << QCharPair(QChar(i), QChar(i - 0xFEE0));
    list << QCharPair(QChar(0x3000), QChar(0x0020));
    return list;
}

// QChar 空格 全角半角对列表
QCharPairList WidthForms::charSpaceList()
{
    QCharPairList list;
    list << QCharPair(QChar(0x3000), QChar(0x0020));
    return list;
}

// QChar 数字 全角半角对列表
QCharPairList WidthForms::charNumberList()
{
    QCharPairList list;
    for(ushort i = 0xFF10; i <= 0xFF19; i++)
        list << QCharPair(QChar(i), QChar(i - 0xFEE0));
    return list;
}

// QChar 字母 全角半角对列表
QCharPairList WidthForms::charCaseList()
{
    QCharPairList list;
    list << charUpperCaseList() << charLowerCaseList();
    return list;
}

// QChar 大写字母 全角半角对列表
QCharPairList WidthForms::charUpperCaseList()
{
    QCharPairList list;
    for(ushort i = 0xFF21; i <= 0xFF3A; i++)
        list << QCharPair(QChar(i), QChar(i - 0xFEE0));
    return list;
}

// QChar 小写字母 全角半角对列表
QCharPairList WidthForms::charLowerCaseList()
{
    QCharPairList list;
    for(ushort i = 0xFF41; i <= 0xFF5A; i++)
        list << QCharPair(QChar(i), QChar(i - 0xFEE0));
    return list;
}

// QChar 常用 小写全角半角对列表（数字、字母、空格）
QCharPairList WidthForms::charCommonList()
{
    QCharPairList list;
    list << charNumberList()
         << charUpperCaseList()
         << charLowerCaseList()
         << charSpaceList();
    return list;
}

// QChar 过滤掉的全角列表（无需转换的），fullSpace: 无需转换的列表中是否包括全角空格，默认不包括
QList<QChar> WidthForms::charFilters(bool fullSpace)
{
    QList<QChar> filters;
    filters << QChar(0xFF01) << QChar(0xFF08) << QChar(0xFF09) << QChar(0xFF0C)
            << QChar(0xFF1A) << QChar(0xFF1B) << QChar(0xFF1F);
    //！（），：；？
    if(fullSpace)
        filters << QChar(0x3000);
    return filters;
}

// QChar 自定义 过滤 全角半角对列表（数字、字母、空格）
QCharPairList WidthForms::charCustomList(QList<QChar> filters)
{
    QCharPairList list;
    for(ushort i = 0xFF01; i <= 0xFF5E; i++) {
        if(!filters.contains(QChar(i)))
            list << QCharPair(QChar(i), QChar(i - 0xFEE0));
    }
    if(!filters.contains(QChar(0x3000)))
        list << QCharPair(QChar(0x3000), QChar(0x0020));
    return list;
}

// QString 所有 全角半角对列表
QStringPairList WidthForms::stringAllList()
{
    QStringPairList list;
    for(ushort i = 0xFF01; i <= 0xFF5E; i++)
        list << QStringPair(QString(QChar(i)), QString(QChar(i - 0xFEE0)));
    list << QStringPair(QString(QChar(0x3000)), QString(QChar(0x0020)));
    return list;
}

// QString 空格 全角半角对列表
QStringPairList WidthForms::stringSpaceList()
{
    QStringPairList list;
    list << QStringPair(QString(QChar(0x3000)), QString(QChar(0x0020)));
    return list;
}

// QString 数字 全角半角对列表
QStringPairList WidthForms::stringNumberList()
{
    QStringPairList list;
    for(ushort i = 0xFF10; i <= 0xFF19; i++)
        list << QStringPair(QString(QChar(i)), QString(QChar(i - 0xFEE0)));
    return list;
}

// QString 字母 全角半角对列表
QStringPairList WidthForms::stringCaseList()
{
    QStringPairList list;
    list << stringUpperCaseList() << stringLowerCaseList();
    return list;
}

// QString 大写字母 全角半角对列表
QStringPairList WidthForms::stringUpperCaseList()
{
    QStringPairList list;
    for(ushort i = 0xFF21; i <= 0xFF3A; i++)
        list << QStringPair(QString(QChar(i)), QString(QChar(i - 0xFEE0)));
    return list;
}

// QString 小写字母 全角半角对列表
QStringPairList WidthForms::stringLowerCaseList()
{
    QStringPairList list;
    for(ushort i = 0xFF41; i <= 0xFF5A; i++)
        list << QStringPair(QString(QChar(i)), QString(QChar(i - 0xFEE0)));
    return list;
}

// QString 常用 小写全角半角对列表（数字、字母、空格）
QStringPairList WidthForms::stringCommonList()
{
    QStringPairList list;
    list << stringNumberList()
         << stringUpperCaseList()
         << stringLowerCaseList()
         << stringSpaceList();
    return list;
}

// QString 过滤掉的全角列表（无需转换的），fullSpace: 无需转换的列表中是否包括全角空格，默认不包括
QStringList WidthForms::stringFilters(bool fullSpace)
{
    QStringList filters;
    filters << QString(QChar(0xFF01)) << QString(QChar(0xFF08))
            << QString(QChar(0xFF09)) << QString(QChar(0xFF0C))
            << QString(QChar(0xFF1A)) << QString(QChar(0xFF1B))
            << QString(QChar(0xFF1F));
    //！（），：；？
    if(fullSpace)
        filters << QString(QChar(0x3000));
    return filters;
}

// QString 自定义 过滤 全角半角对列表（数字、字母、空格）
QStringPairList WidthForms::stringCustomList(QStringList filters)
{
    QStringPairList list;
    for(ushort i = 0xFF01; i <= 0xFF5E; i++) {
        if(!filters.contains(QString(QChar(i))))
            list << QStringPair(QString(QChar(i)), QString(QChar(i - 0xFEE0)));
    }
    if(!filters.contains(QString(QChar(0x3000))))
        list << QStringPair(QString(QChar(0x3000)), QString(QChar(0x0020)));
    return list;
}
