/* Main for project hanoi tower.
 * Jere Aho, jere.aho@tuni.fi
 * */
#include "mainwindow.hh"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
