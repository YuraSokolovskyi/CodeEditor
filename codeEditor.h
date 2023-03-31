#ifndef CODEEDITOR_CODEEDITOR_H
#define CODEEDITOR_CODEEDITOR_H

#include "QPlainTextEdit"

class codeEditor : public QPlainTextEdit {
Q_OBJECT

public:
    QWidget *lineCounterWidget;

    codeEditor(QWidget *parent = nullptr);

    void lineCounterPaintEvent(QPaintEvent *event);
    int lineCounterWidth();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void updateLineCounterWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineCounter(const QRect &rect, int dy);

private:
};


#endif //CODEEDITOR_CODEEDITOR_H
