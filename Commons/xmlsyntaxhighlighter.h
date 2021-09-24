#ifndef XMLSYNTAXHIGHLIGHTER_H
#define XMLSYNTAXHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>

class XmlSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    XmlSyntaxHighlighter(QTextDocument *parent = 0, QString filePath = nullptr);

protected:
    virtual void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat tagFormat;
    QTextCharFormat xmlStartFormat;
    QTextCharFormat attributeFormat;
    QTextCharFormat attributeContentFormat;
    QTextCharFormat commentFormat;

    QTextCharFormat textFormat;
    QTextCharFormat errorFormat;
    QTextCharFormat warnFormat;
    QTextCharFormat fatalFormat;
    QTextCharFormat pathFormat;
    QTextCharFormat coordinateFormat;
};

#endif // XMLSYNTAXHIGHLIGHTER_H
