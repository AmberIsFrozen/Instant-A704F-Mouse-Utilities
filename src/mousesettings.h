#ifndef MOUSESETTINGS_H
#define MOUSESETTINGS_H

#include <QString>
#include <QStandardPaths>

class MouseSettings
{
public:
    MouseSettings();

    const QString CONFIG_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    struct DPIProfile {
        int dpiValue;
        bool enabled;

        int getReadableDPI() const;
    };

    struct MouseButton {
        QString name;
        int buttonHex;
        int bindedAction;
        QKeyCombination keyboardCombo;
        uint16_t multimediaKey = 0;
    };

    enum RGBMode {
        SLOW = 0x10,
        FAST = 0x13,
        FASTER = 0x14,
        VERY_FAST = 0x15,
        STATIC = 0x16,
        OFF = 0x17
    };

    static constexpr char actionMenuIdxToHex[12] = {
        0x01,
        0x02,
        0x03,
        0x04,
        0x05,
        0x06,
        0x07,
        0x08,
        0x09,
        0x0b,
        0x0c,
        0x00
    };

    static const int DPI_PROFILES = 4;
    unsigned char rgbMode = 0x10;
    unsigned char scrollMode = 0x00;
    int fireRate = 4;
    int activeDpi = 0;
    bool applyDpi = true;

    DPIProfile dpiProfiles[DPI_PROFILES] = {
        DPIProfile{3, true},
        DPIProfile{4, true},
        DPIProfile{6, true},
        DPIProfile{12, true}
    };

    MouseButton buttons[7] = {
        MouseButton{"LMB", 0x01, 0}, // LMB
        MouseButton{"MMB", 0x02, 1}, // MMB
        MouseButton{"RMB", 0x03, 2}, // RMB
        MouseButton{"SIDE_FWD", 0x05, 3}, // SIDE_FWD
        MouseButton{"SIDE_BWD", 0x04, 4}, // SIDE_BWD
        MouseButton{"MID_FWD", 0x08, 9}, // MID_FWD
        MouseButton{"MID_BWD", 0x06, 10} // MID_BWD
    };

    void loadFromFile();
    void saveToFile() const;
    DPIProfile getActiveDpiProfile() const;
};

#endif // MOUSESETTINGS_H
