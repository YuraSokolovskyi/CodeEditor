#include <iostream>
#include "C_plus_highlighter.h"
#include "QTextCharFormat"
#include "QBrush"
#include "QColor"

C_plus_highlighter::C_plus_highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    HighlightingRule rule;
    QTextCharFormat format;

    // include
    format.setForeground(QColor(162, 180, 41));
    rule.pattern = QRegularExpression("#include");
    rule.format = format;
    includeRule = rule;

    // <> after include
    format.setForeground(QColor(95, 134, 76));
    rule.pattern = QRegularExpression(R"(<\S+>)");
    rule.format = format;
    afterInclude = rule;

    // string
    format.setForeground(QColor(95, 134, 76));
    rule.pattern = QRegularExpression("\"");
    rule.format = format;
    stringRule = rule;

    // comments
    format.setForeground(QColor(128, 128, 128));
    rule.pattern = QRegularExpression("//");
    rule.format = format;
    commentRule = rule;
    rule.pattern = QRegularExpression(R"(/\*)");
    multilineCommentStartRule = rule;
    rule.pattern = QRegularExpression(R"(\*/)");
    multilineCommentEndRule = rule;

    // digits
    format.setForeground(QColor(89, 138, 177));
    rule.format = format;
    rule.pattern = QRegularExpression(R"(\b\d+\b)");
    digitRule = rule;

    // keywords
    keywordsFormat.setForeground(QColor(204, 120, 50));
    rule.format = keywordsFormat;

    // semicolon
    rule.pattern = QRegularExpression(";");
    semicolonRule = rule;

    // data types
    QVector<QString> *dataTypes = new QVector<QString>({
        R"(\bconst\b)", R"(\bvoid\b)", R"(\bsigned\b)", R"(\bunsigned\b)", R"(\bchar\b)", R"(\bshort\b)",
        R"(\bint\b)", R"(\blong\b)", R"(\bfloat\b)", R"(\bdouble\b)", R"(\bbool\b)", R"(\bclass\b)", R"(\bstruct\b)"
    });
    for (QString i : *dataTypes){
        rule.pattern = QRegularExpression(i);
        dataTypesRules.append(rule);
    }

    // data loops and conditions
    QVector<QString> *loopsConditions = new QVector<QString>({
        R"(\bif\b)", R"(\bfor\b)", R"(\bwhile\b)", R"(\bdo\b)", R"(\bswitch\b)", R"(\belse\b)"
    });
    for (QString i : *loopsConditions){
        rule.pattern = QRegularExpression(i);
        loopsConditionsRules.append(rule);
    }

    // other keywords
    QVector<QString> *otherKeywords = new QVector<QString>({
        R"(\busing\b)", R"(\bnamespace\b)"
    });
    for (QString i : *otherKeywords){
        rule.pattern = QRegularExpression(i);
        otherKeywordsRules.append(rule);
    }

    // functions
    format.setForeground(QColor(255, 198, 109));
    rule.format = format;
    std::string temp = "(";
    for (QString i : *dataTypes){
        temp += "(" + i.toStdString() + ")|";
    }
    temp = temp.substr(0, temp.length() - 1) + ")";
    rule.pattern = QRegularExpression((temp + R"( \w+( )*\()").c_str());
    functionRule = rule;
}

void C_plus_highlighter::highlightBlock(const QString &text){
    setCurrentBlockState(C_plus_highlighter::None);

    int startIndex = 0;
    int endIndex = 0;

    // comments
    startIndex = text.indexOf(commentRule.pattern);
    if (startIndex != -1) {
        setFormat(startIndex, text.length() - startIndex + 1, commentRule.format);
        return;
    }

    // multiline
    startIndex = endIndex = 0;
    if (previousBlockState() != C_plus_highlighter::Comment) startIndex = text.indexOf(multilineCommentStartRule.pattern);
    while (startIndex >= 0){
        QRegularExpressionMatch endMatch;
        endIndex = text.indexOf(multilineCommentEndRule.pattern, startIndex, &endMatch);
        int len;
        if (endIndex == -1){
            setCurrentBlockState(C_plus_highlighter::Comment);
            len = text.length() - startIndex;
        }else{
            len = endIndex - startIndex + endMatch.capturedLength();
        }
        setFormat(startIndex, len, commentRule.format);
        startIndex = text.indexOf(multilineCommentStartRule.pattern, startIndex + len);
    }

    if (currentBlockState() == C_plus_highlighter::Comment) return;

    // include
    startIndex = endIndex = 0;
    if (previousBlockState() != C_plus_highlighter::Include){
        startIndex = text.indexOf(includeRule.pattern);
        setFormat(startIndex, 8, includeRule.format);
    }

    // <> after include
    startIndex = endIndex = 0;
    if (text.indexOf("#include") != -1){
        QRegularExpressionMatch match;
        startIndex = text.indexOf(afterInclude.pattern, 0, &match);
        endIndex = text.indexOf(">", startIndex);
        setFormat(startIndex, endIndex - startIndex + 1, afterInclude.format);
    }

    // string
    startIndex = endIndex = 0;
    if (previousBlockState() != C_plus_highlighter::String){
        startIndex = text.indexOf(stringRule.pattern);
        while (startIndex >= 0){
            QRegularExpressionMatch match;
            endIndex = text.indexOf(stringRule.pattern, startIndex + 1, &match);
            int len = 0;
            if (endIndex != -1) {
                len = endIndex - startIndex + 1;
                setCurrentBlockState(C_plus_highlighter::String);
            }
            else len = text.length() - startIndex;
            setFormat(startIndex, len, stringRule.format);
            startIndex = text.indexOf(stringRule.pattern, startIndex + len);
        }
    }else{
        return;
    }

    // digits
    endIndex = startIndex = 0;
    startIndex = text.indexOf(digitRule.pattern);
    while (startIndex >= 0){
        QRegularExpressionMatch match;
        int temp = text.indexOf(digitRule.pattern, startIndex, &match);
        setFormat(startIndex, match.capturedLength(), digitRule.format);
        startIndex = text.indexOf(digitRule.pattern, startIndex + match.capturedLength());
    }

    // data types
    startIndex = endIndex = 0;
    for (HighlightingRule i : dataTypesRules){
        startIndex = text.indexOf(i.pattern);
        while (startIndex >= 0){
            QRegularExpressionMatch match;
            int temp = text.indexOf(i.pattern, startIndex, &match);
            endIndex = text.indexOf(QRegularExpression(" "), startIndex);
            setFormat(startIndex, endIndex - startIndex, keywordsFormat);
            startIndex = text.indexOf(i.pattern, startIndex + match.capturedLength());
        }
    }

    // loops and conditions
    startIndex = endIndex = 0;
    for (HighlightingRule i : loopsConditionsRules){
        startIndex = text.indexOf(i.pattern);
        while (startIndex >= 0){
            QRegularExpressionMatch match;
            int temp = text.indexOf(i.pattern, startIndex, &match);
            endIndex = text.indexOf(QRegularExpression(R"([ \({])"), startIndex);
            setFormat(startIndex, endIndex - startIndex, keywordsFormat);
            startIndex = text.indexOf(i.pattern, startIndex + match.capturedLength());
        }
    }

    // other keywords
    startIndex = endIndex = 0;
    for (HighlightingRule i : otherKeywordsRules){
        startIndex = text.indexOf(i.pattern);
        while (startIndex >= 0){
            QRegularExpressionMatch match;
            int temp = text.indexOf(i.pattern, startIndex, &match);
            setFormat(startIndex, match.capturedLength(), i.format);
            startIndex = text.indexOf(i.pattern, startIndex + match.capturedLength());
        }
    }

    // semicolons
    startIndex = text.indexOf(semicolonRule.pattern);
    while (startIndex >= 0){
        setFormat(startIndex, 1, keywordsFormat);
        startIndex = text.indexOf(semicolonRule.pattern, startIndex + 1);
    }

    // functions
    startIndex = endIndex = 0;
    startIndex = text.indexOf(functionRule.pattern);
    while (startIndex >= 0){
        QRegularExpressionMatch match;
        int _ = text.indexOf(functionRule.pattern, startIndex, &match);
        int temp = text.indexOf(QRegularExpression(R"( \w)"), startIndex);
        endIndex = text.indexOf(QRegularExpression(R"([( )*\(])"), temp + 1);
        setFormat(temp + 1, endIndex - temp - 1, functionRule.format);
        startIndex = text.indexOf(functionRule.pattern, startIndex + match.capturedLength());
    }
}