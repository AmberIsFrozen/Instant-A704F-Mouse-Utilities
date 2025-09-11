#ifndef HIDHELPER_H
#define HIDHELPER_H

#include "mousesettings.h"
#include <hidapi.h>
#include <QApplication>

class HIDHelper
{
public:
    struct VendorProductID {
        int vendorId;
        int productId;
    };

    static hid_device_* openMouseInterface(QWidget *app);
    static hid_device_* openKeyboardInterface();
    static void sendMouseReport(hid_device *dev, unsigned char updateType, unsigned char forComponent, unsigned char data, unsigned char enabledDpiProfile = 0x00);
    static void sendRawMouseReport(hid_device *dev, unsigned char updateType, unsigned char data[], int dataLength);
    static void applyMouseSettings(hid_device *dev, MouseSettings settings);
    static constexpr VendorProductID supportedDevices[] {
        {0x18f8, 0x1286},
        {0x30fa, 0x1701}
    };

    enum UpdateType {
        DPI = 0x09,
        KeyRebind = 0x10,
        ScrollWheel = 0x11,
        FireRate = 0x12,
        RGB = 0x13,
        MakeInputKeyboard = 0x15,
        MakeInputMediaKey = 0x17
    };
};

#endif // HIDHELPER_H
