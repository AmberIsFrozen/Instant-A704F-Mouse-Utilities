#include "custominputhandler.h"
#include "mainwindow.h"
#include "hidhelper.h"
#include "mousesettings.h"
#include "qmenu.h"
#include <QTimer>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QStyleFactory>
#include <QCommandLineParser>

#define APP_NAME "A704F-Mouse-Utilities"

int parseCLI(QStringList arguments) {
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
        {"apply", "Apply mouse configuration based on the current config file"},
        {"daemon", "Start daemon to perform special action. (Key Input etc.)"}
    });
    parser.process(arguments);

    bool parsed = false;
    bool doExecLoop = false;

    if(parser.isSet("apply")) {
        parsed = true;
        hid_device *dev = HIDHelper::openMouseInterface(NULL);
        if(dev) {
            MouseSettings mouseSettings = MouseSettings();
            mouseSettings.loadFromFile();
            HIDHelper::applyMouseSettings(dev, mouseSettings);
            hid_close(dev);

            qDebug() << "Mouse settings applied.";
        } else {
            qDebug() << "Failed to apply mouse settings.";
        }
    }

    if(parser.isSet("daemon")) {
        parsed = true;
        doExecLoop = true;
        hid_device *keyboardInterface = HIDHelper::openKeyboardInterface();

        if(keyboardInterface) {
            MouseSettings *mouseSettings = new MouseSettings();
            mouseSettings->loadFromFile();

            QThread *inputHandlerThread = new QThread();
            MouseInputHandler *im = new MouseInputHandler(keyboardInterface, mouseSettings);
            im->moveToThread(inputHandlerThread);
            MouseInputHandler::connect(inputHandlerThread, &QThread::started, im, &MouseInputHandler::run);
            MouseInputHandler::connect(inputHandlerThread, &QThread::finished, im, &MouseInputHandler::deleteLater);
            inputHandlerThread->start();

            qDebug() << "Daemon started.";
        } else {
            qCritical() << "Cannot open the keyboard interface (Interface 1). Have you installed the udev rules or run as root?";
            doExecLoop = false;
        }
    }

    return parsed + doExecLoop;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);

    int cliResult = parseCLI(a.arguments());
    if(cliResult == 1) { // Parsed & no need app loop
        return 0;
    } else {
        QMenu *menu = new QMenu();
        QAction *quitAction = menu->addAction("Quit");
        QObject::connect(quitAction, &QAction::triggered, []() {QCoreApplication::quit();});
        QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/img/tray.png"));
        trayIcon->setContextMenu(menu);
        trayIcon->show();

        if(!cliResult) {
            MainWindow w;
            w.show();
            return a.exec();
        }

        return a.exec();
    }
}
