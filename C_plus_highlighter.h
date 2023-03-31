#ifndef CODEEDITOR_C_PLUS_HIGHLIGHTER_H
#define CODEEDITOR_C_PLUS_HIGHLIGHTER_H

#include "QSyntaxHighlighter"
#include "QRegularExpression"


class C_plus_highlighter : public QSyntaxHighlighter{
Q_OBJECT

public:
    explicit C_plus_highlighter(QTextDocument *parent = nullptr);
private:
    enum States{
        None,
        Comment,
        Include,
        String
    };

    struct HighlightingRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    // include rules
    HighlightingRule includeRule;
    HighlightingRule afterInclude; // <>

    // string rules
    HighlightingRule stringRule;

    // comment
    HighlightingRule commentRule;
    HighlightingRule multilineCommentStartRule;
    HighlightingRule multilineCommentEndRule;

    // digits
    HighlightingRule digitRule;

    // keywords
    QTextCharFormat keywordsFormat;
    HighlightingRule semicolonRule; // semicolon
    QVector<HighlightingRule> dataTypesRules; // data types
    QVector<HighlightingRule> loopsConditionsRules; // loops and conditions
    QVector<HighlightingRule> otherKeywordsRules;

    // functions
    HighlightingRule functionRule;


protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
};


#endif //CODEEDITOR_C_PLUS_HIGHLIGHTER_H
