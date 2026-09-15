#include "mainwindow.h"
#include "customkeyboarddialog.h"
#include "custominputhandler.h"
#include "ui_mainwindow.h"
#include "hidhelper.h"
#include <hidapi/hidapi.h>
#include <QMessageBox>
#include <QTimer>
#include <QThread>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    mouseSettings.loadFromFile();

    hid_init();
    ui->setupUi(this);
    ui->disconnectedPage->hide();
    hid_device *dev = HIDHelper::openMouseInterface(this);
    HIDHelper::applyMouseSettings(dev, mouseSettings); // Apply config on startup, since config is not persistently stored on mouse
    hid_close(dev);

    initUIFields(mouseSettings);
    // Mouse ground thread
    inputHandlerThread = new QThread();
    MouseInputHandler *im = new MouseInputHandler(&mouseSettings);
    im->moveToThread(inputHandlerThread);
    connect(im, &MouseInputHandler::input, this, &MainWindow::handleSpecialInput);
    connect(im, &MouseInputHandler::updateMouseStatus, this, &MainWindow::updateMouseStatus);
    connect(inputHandlerThread, &QThread::started, im, &MouseInputHandler::run);
    connect(inputHandlerThread, &QThread::finished, im, &MouseInputHandler::deleteLater);
    inputHandlerThread->start();

    rgbPreview.init(ui->rgb_mouse_display, ui->rgb_mouse_base, { ui->rgb_mouse_red, ui->rgb_mouse_purple, ui->rgb_mouse_blue, ui->rgb_mouse_magenta });

    QTimer *eventLoopTimer = new QTimer(this);
    connect(eventLoopTimer, &QTimer::timeout, this, [this]{onEventLoop();}); // Update event loop
    eventLoopTimer->start(1000/30);
}

MainWindow::~MainWindow()
{
    if(inputHandlerThread) {
        inputHandlerThread->requestInterruption();
        inputHandlerThread->quit();
        inputHandlerThread->wait();
    }
    hid_exit();
    delete ui;
}

void MainWindow::onEventLoop() {
    if(ui->tabWidget->currentIndex() == 1) { // Only preview in RGB Tab
        rgbPreview.draw(mouseSettings.rgbMode);
    }
}

void MainWindow::handleSpecialInput(unsigned char type, unsigned char mouseKeyPressed, unsigned char dpi) {
    if(type == 0x06) { // Button Pressed or Released
        if(dpi != 0x80) { // button not released (Pressed)
            mouseSettings.activeDpi = dpi;
            updateDpiUI();
        }
    }
}

void MainWindow::updateMouseStatus(bool isConnected) {
    if(isConnected) {
        ui->mainPage->show();
        ui->disconnectedPage->hide();
    } else {
        ui->mainPage->hide();
        ui->disconnectedPage->show();
    }
}

void MainWindow::initUIFields(const MouseSettings settings) {
    // RGB
    ui->rgbNone->setChecked(settings.rgbMode == 0x17);
    ui->rgbStatic->setChecked(settings.rgbMode == 0x16);
    ui->rgbSuperFast->setChecked(settings.rgbMode == 0x15);
    ui->rgbFast->setChecked(settings.rgbMode == 0x13);
    ui->rgbSlow->setChecked(settings.rgbMode == 0x10);
    ui->applyCheckBox->setChecked(settings.applyDpi);

    // DPI
    for(int i = 0; i < settings.DPI_PROFILES; i++) {
        QRadioButton* radio = i == 0 ? ui->dpi1Radio : i == 1 ? ui->dpi2Radio : i == 2 ? ui->dpi3Radio : ui->dpi4Radio;
        QCheckBox* enabled_cb = i == 0 ? ui->dpi1Enabled : i == 1 ? ui->dpi2Enabled : i == 2 ? ui->dpi3Enabled : ui->dpi4Enabled;
        radio->setChecked(i == settings.activeDpi);
        enabled_cb->setChecked(settings.dpiProfiles[i].enabled);
    }

    updateDpiUI();

    // Scroll Mode
    ui->scrollRadio->setChecked(settings.scrollMode == 0x00);
    ui->volumeRadio->setChecked(settings.scrollMode == 0x01);

    // Fire Rate
    ui->fireRateSlider->setValue(settings.fireRate);

    // Bindings
    ui->lmbComboBox->setCurrentIndex(settings.buttons[0].bindedAction);
    ui->mmbComboBox->setCurrentIndex(settings.buttons[1].bindedAction);
    ui->rmbComboBox->setCurrentIndex(settings.buttons[2].bindedAction);
    ui->sfdComboBox->setCurrentIndex(settings.buttons[3].bindedAction);
    ui->sbdComboBox->setCurrentIndex(settings.buttons[4].bindedAction);
    ui->mfdComboBox->setCurrentIndex(settings.buttons[5].bindedAction);
    ui->mbdComboBox->setCurrentIndex(settings.buttons[6].bindedAction);
}

void MainWindow::promptCustomBindingDialog(int buttonIndex) {
    MouseSettings::MouseButton &btn = mouseSettings.buttons[buttonIndex];
    CustomKeyboardDialog *dialog = new CustomKeyboardDialog(btn, this);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_rgbSlow_clicked()
{
    mouseSettings.rgbMode = MouseSettings::RGBMode::SLOW;
}

void MainWindow::on_rgbFast_clicked()
{
    mouseSettings.rgbMode = MouseSettings::RGBMode::FAST;
}

void MainWindow::on_rgbSuperFast_clicked()
{
    mouseSettings.rgbMode = MouseSettings::RGBMode::VERY_FAST;
}

void MainWindow::on_rgbStatic_clicked()
{
    mouseSettings.rgbMode = MouseSettings::RGBMode::STATIC;
}


void MainWindow::on_rgbNone_clicked()
{
    mouseSettings.rgbMode = MouseSettings::RGBMode::OFF;
}


void MainWindow::on_applyButton_clicked()
{
    hid_device *dev = HIDHelper::openMouseInterface(this);
    HIDHelper::applyMouseSettings(dev, mouseSettings);
    hid_close(dev);
    mouseSettings.saveToFile();
}

void MainWindow::on_lmbComboBox_activated(int index)
{
    mouseSettings.buttons[0].bindedAction = index;
    if(index == 11) promptCustomBindingDialog(0);
}

void MainWindow::on_mmbComboBox_activated(int index)
{
    mouseSettings.buttons[1].bindedAction = index;
    if(index == 11) promptCustomBindingDialog(1);
}

void MainWindow::on_rmbComboBox_activated(int index)
{
    mouseSettings.buttons[2].bindedAction = index;
    if(index == 11) promptCustomBindingDialog(2);
}

void MainWindow::on_sfdComboBox_activated(int index)
{
    mouseSettings.buttons[3].bindedAction = index;
    if(index == 11) promptCustomBindingDialog(3);
}

void MainWindow::on_sbdComboBox_activated(int index)
{
    mouseSettings.buttons[4].bindedAction = index;
    if(index == 11) promptCustomBindingDialog(4);
}

void MainWindow::on_mfdComboBox_activated(int index)
{
    mouseSettings.buttons[5].bindedAction = index;
    if(index == 11) promptCustomBindingDialog(5);
}

void MainWindow::on_mbdComboBox_activated(int index)
{
    mouseSettings.buttons[6].bindedAction = index;
    promptCustomBindingDialog(6);
}

void MainWindow::on_flashRGBButton_clicked()
{
    // Start flashing
    for(int i = 0; i < 30; i++) {
        QTimer::singleShot(i * 100, this, [this, i]{
            hid_device *dev = HIDHelper::openMouseInterface(this);
            HIDHelper::sendMouseReport(dev, HIDHelper::RGB, 0x7f, i % 2 != 0 ? mouseSettings.rgbMode : 0x17);
            hid_close(dev);
        });
    }
}

void MainWindow::updateDpiUI() {
    for(int i = 0; i < mouseSettings.DPI_PROFILES; i++) {
        QRadioButton* radio = i == 0 ? ui->dpi1Radio : i == 1 ? ui->dpi2Radio : i == 2 ? ui->dpi3Radio : ui->dpi4Radio;
        QLabel* label = i == 0 ? ui->dpi1Value : i == 1 ? ui->dpi2Value : i == 2 ? ui->dpi3Value : ui->dpi4Value;
        radio->setChecked(i == mouseSettings.activeDpi);
        label->setText(QString::number(mouseSettings.dpiProfiles[i].getReadableDPI()));
    }

    ui->dpiSlider->setValue(mouseSettings.getActiveDpiProfile().dpiValue);
}


void MainWindow::on_dpi1Radio_clicked()
{
    mouseSettings.activeDpi = 0;
    updateDpiUI();
}


void MainWindow::on_dpi2Radio_clicked()
{
    mouseSettings.activeDpi = 1;
    updateDpiUI();
}


void MainWindow::on_dpi3Radio_clicked()
{
    mouseSettings.activeDpi = 2;
    updateDpiUI();
}


void MainWindow::on_dpi4Radio_clicked()
{
    mouseSettings.activeDpi = 3;
    updateDpiUI();
}


void MainWindow::on_dpiSlider_valueChanged(int value)
{
    mouseSettings.dpiProfiles[mouseSettings.activeDpi].dpiValue = value;
    updateDpiUI();
}


void MainWindow::on_fireRateSlider_valueChanged(int value)
{
    mouseSettings.fireRate = value;
    double fireRatePercentage = value / 255.0;
    if(fireRatePercentage < 0.5) {
        ui->fireRateLabel->setText(tr("Every %1ms").arg(QString::number(fireRatePercentage * 2000, 'f', 0)));
    } else {
        ui->fireRateLabel->setText(tr("Every %1s").arg(QString::number(fireRatePercentage * 2, 'f', 2)));
    }
}

void MainWindow::on_dpi1Enabled_stateChanged(int arg1)
{
    mouseSettings.dpiProfiles[0].enabled = arg1 == Qt::Checked;
}


void MainWindow::on_dpi2Enabled_stateChanged(int arg1)
{
    mouseSettings.dpiProfiles[1].enabled = arg1 == Qt::Checked;
}


void MainWindow::on_dpi3Enabled_stateChanged(int arg1)
{
    mouseSettings.dpiProfiles[2].enabled = arg1 == Qt::Checked;
}


void MainWindow::on_dpi4Enabled_stateChanged(int arg1)
{
    mouseSettings.dpiProfiles[3].enabled = arg1 == Qt::Checked;
}


void MainWindow::on_resetButton_clicked()
{
    MouseSettings freshMouseSettings = MouseSettings();
    initUIFields(freshMouseSettings);

    hid_device *dev = HIDHelper::openMouseInterface(this);
    HIDHelper::applyMouseSettings(dev, freshMouseSettings);
    hid_close(dev);

    freshMouseSettings.saveToFile();
    mouseSettings.loadFromFile();
}

void MainWindow::on_scrollRadio_clicked(bool checked)
{
    if(checked) {
        mouseSettings.scrollMode = 0x00;
    }
}


void MainWindow::on_volumeRadio_clicked(bool checked)
{
    if(checked) {
        mouseSettings.scrollMode = 0x01;
    }
}

void MainWindow::on_applyCheckBox_clicked(bool checked) {
    mouseSettings.applyDpi = checked;
}
