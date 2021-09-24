#include "xmlsyntaxhighlighter.h"
#include <QDebug>

XmlSyntaxHighlighter::XmlSyntaxHighlighter(QTextDocument *parent, QString filePath)
    :QSyntaxHighlighter(parent)
{
    Q_UNUSED(filePath)
    HighlightingRule rule;

    textFormat.setForeground(Qt::darkGreen);
    textFormat.setFontPointSize(12);
    rule.pattern = QRegExp(".");
    rule.format = textFormat;
    highlightingRules.append(rule);

    errorFormat.setForeground(Qt::red);
    errorFormat.setFontPointSize(12);
    rule.pattern = QRegExp("错误：.+");
    rule.format = errorFormat;
    highlightingRules.append(rule);

    warnFormat.setForeground(Qt::red);
    warnFormat.setFontPointSize(12);
    rule.pattern = QRegExp("失败：.+");
    rule.format = warnFormat;
    highlightingRules.append(rule);

    fatalFormat.setForeground(Qt::blue);
    fatalFormat.setFontPointSize(12);
    rule.pattern = QRegExp("成功：.+");
    rule.format = fatalFormat;
    highlightingRules.append(rule);

    coordinateFormat.setForeground(Qt::darkYellow);
    coordinateFormat.setFontPointSize(12);
    rule.pattern = QRegExp("提示：.+");
    rule.format = coordinateFormat;
    highlightingRules.append(rule);
}

void XmlSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }

    setCurrentBlockState(0);

//    int startIndex = 0;
//    if (previousBlockState() != 1)
//        startIndex = text.indexOf(commentStartExpression);

//    while (startIndex >= 0)
//    {
//        int endIndex = text.indexOf(commentEndExpression, startIndex);
//        int commentLength;
//        if (endIndex == -1)
//        {
//            setCurrentBlockState(1);
//            commentLength = text.length() - startIndex;
//        }
//        else
//            commentLength = endIndex - startIndex
//                            + commentEndExpression.matchedLength();
//        setFormat(startIndex, commentLength, commentFormat);
//        startIndex = text.indexOf(commentStartExpression,
//                                                startIndex + commentLength);
//    }
}
