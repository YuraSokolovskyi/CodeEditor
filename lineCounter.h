#ifndef CODEEDITOR_LINECOUNTER_H
#define CODEEDITOR_LINECOUNTER_H

#include "QWidget"
#include "codeEditor.h"

class lineCounter : public QWidget {
public:
    lineCounter(codeEditor *editor) : QWidget(editor) {
            codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineCounterWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineCounterPaintEvent(event);
    }

private:
    codeEditor *codeEditor;
};


#endif //CODEEDITOR_LINECOUNTER_H
