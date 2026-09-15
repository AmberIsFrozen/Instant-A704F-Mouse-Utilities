#ifndef INPUTMONITORINGTHREAD_H
#define INPUTMONITORINGTHREAD_H

#include "mousesettings.h"
#include <QThread>
#include <hidapi/hidapi.h>


class MouseInputHandler : public QObject
{
    Q_OBJECT
public:
    MouseInputHandler(MouseSettings *mouseSettings);
    ~MouseInputHandler();
    void run();
signals:
    void input(unsigned char inputType, unsigned char mouseKey, unsigned char dpi);
    void updateMouseStatus(bool isConnected);
private:
    hid_device *device;
    MouseSettings *mouseSettings;
    void sendCustomKeyInput(hid_device *device, QKeyCombination keyCombo);
    void sendMultimediaKey(hid_device *device, uint16_t key);
};

#endif // INPUTMONITORINGTHREAD_H
