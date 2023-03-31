#include <QTextBlock>
#include <QScrollBar>
#include "codeEditor.h"
#include "QPainter"
#include "lineCounter.h"
#include "iostream"

codeEditor::codeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    this->setLineWrapMode(NoWrap);

    QFont temp = this->font();
    temp.setPointSize(18);
    this->setFont(temp);

    lineCounterWidget = new lineCounter(this);
    lineCounterWidget->setObjectName("lineCounter");
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineCounterWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineCounter(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineCounterWidth(0);
    highlightCurrentLine();
}

int codeEditor::lineCounterWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().averageCharWidth() * digits;

    if (space < 20) return 20;

    return space;
}

void codeEditor::updateLineCounter(const QRect &rect, int dy) {
    if (dy)
        lineCounterWidget->scroll(0, dy);
    else
        lineCounterWidget->update(0, rect.y(), lineCounterWidget->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineCounterWidth(0);
}

void codeEditor::updateLineCounterWidth(int newBlockCount) {
    setViewportMargins(lineCounterWidth(), 0, 0, 0);
}

void codeEditor::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineCounterWidget->setGeometry(QRect(cr.left(), cr.top(), lineCounterWidth(), cr.height()));
}

void codeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(QColor(50, 50, 50)); // TODO theme

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void codeEditor::lineCounterPaintEvent(QPaintEvent *event) {
    QPainter painter(lineCounterWidget);
    painter.setFont(font());
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(0, top, lineCounterWidget->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
