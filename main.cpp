#include <QApplication>
#include <QPushButton>
#include "mainwindow.h"
#include "iostream"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QFile styles(R"(..\styles\styles.css)");
    if(styles.open(QIODevice::ReadOnly)){
        QTextStream textStream(&styles);
        QString styleText = textStream.readAll();
        styles.close();
        app.setStyleSheet(styleText);
    }

    mainWindow window;
    window.show();
    return app.exec();
}
