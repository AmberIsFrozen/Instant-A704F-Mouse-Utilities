#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "rgbpreview.h"
#include "mousesettings.h"
#include <QMainWindow>
#include <hidapi/hidapi.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateDpiUI();

private slots:

    void on_rgbSlow_clicked();

    void on_rgbFast_clicked();

    void on_rgbStatic_clicked();

    void on_rgbNone_clicked();

    void on_applyButton_clicked();

    void on_lmbComboBox_activated(int index);

    void on_rmbComboBox_activated(int index);

    void on_mmbComboBox_activated(int index);

    void on_mfdComboBox_activated(int index);

    void on_mbdComboBox_activated(int index);

    void on_sfdComboBox_activated(int index);

    void on_sbdComboBox_activated(int index);

    void on_flashRGBButton_clicked();

    void on_dpi1Radio_clicked();

    void on_dpi2Radio_clicked();

    void on_dpi3Radio_clicked();

    void on_dpi4Radio_clicked();

    void on_dpiSlider_valueChanged(int value);

    void on_fireRateSlider_valueChanged(int value);

    void on_dpi1Enabled_stateChanged(int arg1);

    void on_dpi2Enabled_stateChanged(int arg1);

    void on_dpi3Enabled_stateChanged(int arg1);

    void on_dpi4Enabled_stateChanged(int arg1);

    void on_rgbSuperFast_clicked();

    void on_resetButton_clicked();

    void on_scrollRadio_clicked(bool checked);

    void on_volumeRadio_clicked(bool checked);

    void on_applyCheckBox_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QThread *inputHandlerThread;
    RGBPreview rgbPreview;
    MouseSettings mouseSettings = MouseSettings();

    void onEventLoop();
    char getActionType(int index);
    void initUIFields(const MouseSettings settings);
    void promptCustomBindingDialog(int buttonIndex);
    void handleSpecialInput(unsigned char type, unsigned char mouseKeyPressed, unsigned char dpi);
    void updateMouseStatus(bool isConnected);
};
#endif // MAINWINDOW_H
