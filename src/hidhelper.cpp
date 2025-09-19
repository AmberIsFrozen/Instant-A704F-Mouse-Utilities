#include "hidhelper.h"
#include "mousesettings.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <unistd.h>

#define UDEV_RULES "51-a704f-usb-gaming-mouse.rules"

hid_device_* HIDHelper::openMouseInterface(QWidget *parent) {
    hid_device *handle;

    for(VendorProductID vpid : supportedDevices) {
        handle = hid_open(vpid.vendorId, vpid.productId, NULL);
        if(handle) break;
    }

    if(!handle) {
        qCritical("Cannot find device with the designated Vendor/Product ID");
        QMessageBox::critical(parent, "Error", QString("Cannot find the corresponding device.\nIf the device is plugged in, it is likely you need to run this program under root.\nThe program will then prompt you to install an udev rules so you do not have to elevate the application again in the future."));
    }

    if(geteuid() == 0) { // Root UID
        QMessageBox::StandardButton response = QMessageBox::question(parent, "Running under root permission", "You are running this program under root permission. You can avoid that by installing a udev rules.\n\nAfter the installation, you can re-plug your mouse or restart your computer, then you should be able to use the program under normal user account.\n\nDo you want to install it?", QMessageBox::Yes|QMessageBox::No);
        if(response == QMessageBox::StandardButton::Yes) {
            QString resourcePath = QString(":/") + UDEV_RULES;
            QString destinatedPath = QString("/etc/udev/rules.d/") + UDEV_RULES;
            QFile::copy(resourcePath, destinatedPath);
            QMessageBox::information(parent, "Rules installed", "udev rules installed! You should no longer need to run this program as root after re-plugging your device or restarting your computer.");
            qDebug() << "udev rules installed at " << destinatedPath;
        }
    }

    return handle;
}

hid_device_* HIDHelper::openKeyboardInterface() {
    hid_device_info *infos;
    for(VendorProductID vpid : supportedDevices) {
        infos = hid_enumerate(vpid.vendorId, vpid.productId);
        if(infos) break;
    }

    hid_device_info *info = infos;
    char* targetInterface = nullptr;

    while(info) {
        qDebug() << "Interface" << info->interface_number << "at" << info->path;
        if(info->interface_number == 1) {
            qDebug() << "Interface 1 found";
            targetInterface = info->path;
            break;
        }
        info = info->next;
    }

    if(targetInterface) {
        hid_device *handle = hid_open_path(targetInterface);
        return handle;
    }
    return nullptr;
}

void HIDHelper::sendMouseReport(hid_device *dev, unsigned char updateType, unsigned char forComponent, unsigned char data, unsigned char enabledDpiProfile) {
    unsigned char byteData[6]{0};
    byteData[0] = forComponent;
    byteData[1] = data;
    byteData[2] = enabledDpiProfile;

    sendRawMouseReport(dev, updateType, byteData, 3);
}

void HIDHelper::sendRawMouseReport(hid_device *dev, unsigned char updateType, unsigned char data[6], int dataLength) {
    unsigned char bytes[16]{0};
    bytes[0] = 0x21; // bmRequestType
    bytes[1] = 0x09; // bRequest SET_CONFIGURATION
    bytes[2] = 0x07; // wValue (High)
    bytes[3] = 0x03; // wValue (Low)
    bytes[4] = 0x01; // wIndex
    bytes[5] = 0x00;
    bytes[6] = 0x08; // wLength
    bytes[7] = 0x00;

    bytes[8] = 0x07;
    bytes[9] = updateType;

    for(int i = 0; i < dataLength; i++) {
        bytes[10+i] = data[i];
    }

    hid_send_feature_report(dev, bytes, 16);
}

void HIDHelper::applyMouseSettings(hid_device *dev, MouseSettings settings) {
    // Apply data
    // RGB
    sendMouseReport(dev, RGB, 0x7f, settings.rgbMode);

    // Scroll wheel
    sendMouseReport(dev, ScrollWheel, settings.scrollMode, 0x00);

    // Fire Rate
    sendMouseReport(dev, FireRate, 0x00, settings.fireRate);

    // Mouse bindings
    for(int i = 0; i < 7; i++) {
        sendMouseReport(dev, KeyRebind, settings.buttons[i].buttonHex, MouseSettings::actionMenuIdxToHex[settings.buttons[i].bindedAction]);
    }

    // DPI
    int enabledDpiBit = 0;
    for(int i = 0; i < settings.DPI_PROFILES; i++) {
        enabledDpiBit += settings.dpiProfiles[i].enabled ? 0b1 << i : 0b0 << i;
    }

    for(int i = 0; i < settings.DPI_PROFILES; i++) {
        MouseSettings::DPIProfile dpiProfile = settings.dpiProfiles[i];
        int profileBits = 0x08 + i;
        sendMouseReport(dev, DPI, settings.activeDpi, (dpiProfile.dpiValue << 4) | profileBits, enabledDpiBit);
    }
}
