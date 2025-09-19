#include "custominputhandler.h"
#include "hidhelper.h"
#include "data.cpp"
#include <QThread>
#include <hidapi/hidapi.h>
#include <unistd.h>

MouseInputHandler::MouseInputHandler(hid_device *device, MouseSettings *mouseSettings)
{
    this->device = device;
    this->mouseSettings = mouseSettings;
}

MouseInputHandler::~MouseInputHandler()
{
    hid_close(device);
}

void MouseInputHandler::run() {
    unsigned char buf[10];
    qDebug() << "Started monitoring keyboard interface";

    while(!QThread::currentThread()->isInterruptionRequested()) {
        int res = hid_read_timeout(device, buf, 10, 1000);

        if(res > 0) {
            unsigned char type = buf[0];
            unsigned char dpi = buf[2];
            unsigned char btnCode = buf[1] - 0xf0;
            emit input(type, btnCode, dpi); // Send Signal to UI

            if(dpi == 0x80) continue; // No need process mouse release

            for(MouseSettings::MouseButton mButton : mouseSettings->buttons) {
                if(btnCode != mButton.buttonHex) continue;

                if(mButton.keyboardCombo.toCombined() != Qt::Key_unknown) { // Custom key combo is defined
                    sendCustomKeyInput(device, mButton.keyboardCombo);
                } else if(mButton.multimediaKey != 0) { // Multimedia key is defined
                    sendMultimediaKey(device, mButton.multimediaKey);
                }
            }
        }
    }
    qDebug() << "Stop monitoring for keyboard interface";
}

void MouseInputHandler::sendCustomKeyInput(hid_device *device, QKeyCombination keyCombo) {
    QList<unsigned char> inputList;
    for(int key : DataReference::getModifiers(keyCombo.keyboardModifiers())) {
        inputList.append(key);
    }
    inputList.append(DataReference::qt_key_to_hid_keycode.value(keyCombo.key()));

    int keyCount = inputList.size();
    unsigned char pushedButtons[keyCount];

    // Push one by one
    for(int i = 0; i < keyCount; i++) {
        pushedButtons[i] = inputList[i];
        HIDHelper::sendRawMouseReport(device, HIDHelper::MakeInputKeyboard, pushedButtons, keyCount);
        QThread::msleep(12); // Don't send input too quick
    }

    // Pop one by one
    for(int i = 0; i < keyCount; i++) {
        pushedButtons[(keyCount-1) - i] = 0x00;
        HIDHelper::sendRawMouseReport(device, HIDHelper::MakeInputKeyboard, pushedButtons, keyCount);
        QThread::msleep(12); // Don't send input too quick
    }
}

void MouseInputHandler::sendMultimediaKey(hid_device *device, uint16_t key) {
    unsigned char reports[2];
    // Split to upper byte and lower byte
    reports[0] = key >> 8;
    reports[1] = key & 0x00ff;

    HIDHelper::sendRawMouseReport(device, HIDHelper::MakeInputMediaKey, reports, 4);
}
