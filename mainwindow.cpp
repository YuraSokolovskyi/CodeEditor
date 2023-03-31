#include "mainwindow.h"
#include "ui_mainWindow.h"
#include "QMenuBar"
#include <QMessageBox>
#include "iostream"
#include <fstream>
#include <QShortcut>
#include <QCommonStyle>


mainWindow::mainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::mainWindow) {
    // TODO set jetbrains mono font

    ui->setupUi(this);

    // create editor
    editor = new codeEditor(this);
    editor->setObjectName("codeEdit");

    editor->clear(); // clearing code editor
    createMenu(); // setting up top menu

    m_C_plus_highlighter = new C_plus_highlighter(editor->document()); // setting up highlighter
    fontSize = editor->font().pointSize(); // setting up font of code editor



    //QObject::connect(ui->codeEdit, &QTextEdit::textChanged, this, &mainWindow::codeChanged);
}

mainWindow::~mainWindow() {
    delete ui;
}

void mainWindow::resizeEvent(QResizeEvent *event) { // main window resized
    mainSizeChanged(this->width(), this->height());
    QWidget::resizeEvent(event);
}

void mainWindow::mainSizeChanged(int width, int height) {
    // configure code edit window
    editor->resize(width, height - menu->height()); // change width and height
    editor->move(0, menu->height()); // change position

    // configure main menu
    menu->resize(this->width(), menu->height());
}

void mainWindow::createMenu() {
    menu = new QMenuBar(this); // create menu bar
    menu->setObjectName("mainMenu");

    //create actions
    QAction *newAct = new QAction("&New");
    QAction *openAct = new QAction("&Open");
    QAction *saveAct = new QAction("&Save");
    QAction *saveAsAct = new QAction("S&ave as");
    QAction *testAct = new QAction("&Test option"); // test
    testAct->setObjectName("disabledMenuOption"); // test

    // add shortcuts
    newAct->setShortcut(Qt::CTRL | Qt::Key_N);
    openAct->setShortcut(Qt::CTRL | Qt::Key_O);
    saveAct->setShortcut(Qt::CTRL | Qt::Key_S);
    saveAsAct->setShortcut(Qt::CTRL | Qt::SHIFT | Qt::Key_A);

    // create sub-menus
    QMenu *fileMenu = new QMenu(tr("&File"));

    // create options for sub-menu
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(testAct); // test

    // add sub-menus to main menu
    menu->addMenu(fileMenu);

    // set settings of the main menu
    menu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    menu->resize(100, 20);

    // set connections
    connect(newAct, &QAction::triggered, this, &mainWindow::newFile);
    connect(openAct, &QAction::triggered, this, &mainWindow::openFile);
    connect(saveAct, &QAction::triggered, this, &mainWindow::saveFile);
    connect(saveAsAct, &QAction::triggered, this, &mainWindow::saveAsFile);
}

void mainWindow::openFile() {
    QStringList fileNames;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec()) fileNames = dialog.selectedFiles();
    if (fileNames.length() == 0) return;

    std::ifstream file(fileNames[0].toStdString());
    std::string buffer;
    if (file){
        editor->clear();
        fileName = fileNames[0].toStdString();
    }
    while (std::getline(file, buffer)){
        //ui->codeEdit->append(buffer.c_str());
        editor->appendPlainText(buffer.c_str());
    }
    file.close();
}

void mainWindow::newFile() {
    if (!saved && !editor->toPlainText().toStdString().empty()){
        QMessageBox msgBox;
        msgBox.setText("Do you want to save file?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Save) saveFile();
    }
    fileName = "";
    saved = false;
    editor->clear();
}

void mainWindow::saveFile() {
    if (fileName.empty()){
        saveAsFile();
        return;
    }
    std::ofstream file(fileName, std::ios::trunc);
    QString text = editor->toPlainText();
    file << text.toStdString();
    file.close();
    saved = true;
}

void mainWindow::saveAsFile() {
    QString name = QFileDialog::getSaveFileName(this, QString(), QString(), "Files (*.txt *.py *.cpp *.h)");
    //std::string prefix = fileName.substr(fileName.find_last_of('.'));
    if (name.toStdString().empty()) return;
    fileName = name.toStdString();
    saveFile();
}

void mainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers().testFlag(Qt::ControlModifier)) {
        if (event->angleDelta().y() > 0) { // wheel up
            fontSize++;
            QFont temp = editor->font();
            temp.setPointSize(fontSize);
            editor->setFont(temp);
            editor->lineCounterWidget->repaint();
        }else if (event->angleDelta().y() < 0){ // wheel down
            if (fontSize == 1) return;
            fontSize--;
            QFont temp = editor->font();
            temp.setPointSize(fontSize);
            editor->setFont(temp);
            editor->lineCounterWidget->repaint();
        }
    }
    event->accept();
}

void mainWindow::closeEvent(QCloseEvent *event) {
    if (!saved && !editor->toPlainText().toStdString().empty()){
        QMessageBox msgBox;
        msgBox.setText("Do you want to save file?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Save) saveFile();
    }
    event->accept();
}

