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

enum CLIResult {
    LAUNCH_DAEMON,
    LAUNCH_WINDOW,
    QUIT_GRACEFUL,
    QUIT_ERROR
};

CLIResult parseCLI(QStringList arguments) {
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
        {"apply", "Apply mouse configuration based on the current config file"},
        {"daemon", "Start daemon to perform special action. (Key Input etc.)"}
    });
    parser.process(arguments);

    bool hasApply = false;
    bool runDaemon = false;
    hid_device *dev = HIDHelper::openMouseInterface(NULL);
    if(!dev) {
        qDebug() << "Failed to find mouse device, aborting!";
        return CLIResult::QUIT_ERROR;
    }

    if(parser.isSet("apply")) {
        hasApply = true;

        MouseSettings mouseSettings = MouseSettings();
        mouseSettings.loadFromFile();
        HIDHelper::applyMouseSettings(dev, mouseSettings);

        qDebug() << "Mouse settings applied.";
    }

    if(parser.isSet("daemon")) {
        hasApply = true;
        runDaemon = true;
        hid_device *keyboardInterface = HIDHelper::openKeyboardInterface();

        if(keyboardInterface) {
            qDebug() << "Daemon is set, not spawning window.";
            hid_close(keyboardInterface);
        } else {
            qCritical() << "Cannot open the keyboard interface (Interface 1). Have you installed the udev rules or run as root?";
            runDaemon = false;
        }
    }
    hid_close(dev);

    if(hasApply && !runDaemon) {
        return CLIResult::QUIT_GRACEFUL;
    } else if(hasApply && runDaemon) {
        return CLIResult::LAUNCH_DAEMON;
    } else {
        return CLIResult::LAUNCH_WINDOW;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);

    CLIResult cliResult = parseCLI(a.arguments());
    if(cliResult == CLIResult::QUIT_GRACEFUL) {
        return 0;
    } if(cliResult == CLIResult::QUIT_ERROR) {
        return 1;
    } else {
        MainWindow w;
        QMenu *menu = new QMenu();
        QAction *showAction = menu->addAction("Configure...");
        QObject::connect(showAction, &QAction::triggered, [&]() {w.show();});
        QAction *quitAction = menu->addAction("Quit");
        QObject::connect(quitAction, &QAction::triggered, []() {QCoreApplication::quit();});
        QSystemTrayIcon *trayIcon = new QSystemTrayIcon(QIcon(":/img/tray.png"));
        trayIcon->setContextMenu(menu);
        trayIcon->show();

        if(cliResult == CLIResult::LAUNCH_WINDOW) { // No CLI Flag
            w.show();
        }

        return a.exec();
    }
}
