#include "hidhelper.h"
#include "mousesettings.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <unistd.h>
#include <algorithm>

#define UDEV_RULES "51-a704f-usb-gaming-mouse.rules"

hid_device_* HIDHelper::openMouseInterface(QWidget *parent) {
    hid_device *handle = openKeyboardInterface();

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
    for(const VendorProductID &vpid : supportedDevices) {
        hid_device_info *infos = hid_enumerate(vpid.vendorId, vpid.productId);
        hid_device *handle = nullptr;
        for(hid_device_info *info = infos; info; info = info->next) {
            if(info->interface_number == 1) {
                handle = hid_open_path(info->path);
                if(handle) break;
            }
        }
        hid_free_enumeration(infos);
        if(handle) return handle;
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
    if(!dev || dataLength < 0 || dataLength > 6 || (dataLength && !data)) {
        qWarning("Invalid mouse feature report");
        return;
    }

    // HIDAPI supplies the USB setup packet. Only pass the report ID and payload.
    unsigned char report[8]{0x07, updateType};
    if(dataLength) std::copy_n(data, dataLength, report + 2);
    if(hid_send_feature_report(dev, report, sizeof(report)) != sizeof(report)) {
        qWarning() << "Failed to send mouse feature report:" << hid_error(dev);
    }
}

void HIDHelper::applyMouseSettings(hid_device *dev, MouseSettings settings) {
    if(!dev) return;

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
    if(settings.applyDpi) {
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
}
