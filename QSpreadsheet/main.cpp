#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QDialog>
#include <QSplashScreen>

#include "mainwindow.h"
#include "gotocelldialog.h"
#include "finddialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *startSplash = new QSplashScreen;
    //TODO: Make a new start splash screen image
    startSplash->setPixmap(QPixmap("images/startSplash.png"));
    startSplash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    startSplash->showMessage(QObject::tr("Setting up the main window..."), topRight, Qt::white);
    mainWindow *window = new mainWindow;
    window->setWindowTitle(QObject::tr(VERSION_STRING));

    startSplash->showMessage(QObject::tr("Loading modiles..."), topRight, Qt::white);
    //loadModules();

    startSplash->showMessage(QObject::tr("Establishing connections..."), topRight, Qt::white);
    //establishConnections();
    window->show();
    startSplash->finish(window);
    delete startSplash;

    return a.exec();
}
