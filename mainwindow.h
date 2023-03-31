#ifndef CODEEDITOR_MAINWINDOW_H
#define CODEEDITOR_MAINWINDOW_H

#include <QWidget>
#include "QMenuBar"
#include "QMainWindow"
#include "QFileDialog"
#include "C_plus_highlighter.h"
#include "codeEditor.h"


QT_BEGIN_NAMESPACE
namespace Ui { class mainWindow; }
QT_END_NAMESPACE


class mainWindow : public QWidget {
Q_OBJECT
public:
    explicit mainWindow(QWidget *parent = nullptr);
    ~mainWindow() override;

    std::string fileName;
    bool saved = false;
    int fontSize;

public slots:
private:
    Ui::mainWindow *ui;
    QMenuBar *menu;
    codeEditor *editor;
    C_plus_highlighter *m_C_plus_highlighter;

    void mainSizeChanged(int, int);
    void createMenu();
    void openFile();
    void newFile();
    void saveFile();
    void saveAsFile();

protected:
    void resizeEvent(QResizeEvent*) override;
    void wheelEvent(QWheelEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};


#endif //CODEEDITOR_MAINWINDOW_H
