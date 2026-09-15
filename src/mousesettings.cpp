#include "mousesettings.h"
#include <QString>
#include <QSettings>

MouseSettings::MouseSettings()
{
}

int MouseSettings::DPIProfile::getReadableDPI() const {
    return this->dpiValue * 200;
}

void MouseSettings::loadFromFile() {
    qDebug() << "Reading config from " << CONFIG_PATH;
    QSettings settings(CONFIG_PATH + "/options.ini", QSettings::NativeFormat);
    rgbMode = settings.value("rgb", 0x10).toInt();
    scrollMode = settings.value("scrollMode", 0x00).toInt();

    // Bindings
    settings.beginGroup("Bindings");
    int bindings_size = settings.beginReadArray("bindings");
    for(int i = 0; i < bindings_size; i++) {
        settings.setArrayIndex(i);
        buttons[i].bindedAction = settings.value("action", buttons[i].bindedAction).toInt();
        buttons[i].keyboardCombo = QKeyCombination::fromCombined(settings.value("combo", 0).toInt());
        buttons[i].multimediaKey = settings.value("specialKey", 0).toInt();
    }
    settings.endArray();
    settings.endGroup();

    // DPI
    settings.beginGroup("DPI");
    applyDpi = settings.value("apply", applyDpi).toBool();
    activeDpi = settings.value("active_profile", activeDpi).toInt();
    int size = settings.beginReadArray("profiles");
    for(int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        int dpiValue = settings.value("value", dpiProfiles[i].dpiValue).toInt();
        bool dpiEnabled = settings.value("enabled", dpiProfiles[i].enabled).toBool();
        dpiProfiles[i] = DPIProfile{dpiValue, dpiEnabled};
    }
    settings.endArray();
    settings.endGroup();

    settings.beginGroup("Misc");
    settings.value("fire_rate", fireRate);
    settings.endGroup();
}

void MouseSettings::saveToFile() const {
    QSettings settings(CONFIG_PATH + "/options.ini", QSettings::NativeFormat);
    settings.setValue("rgb", rgbMode);
    settings.setValue("scrollMode", scrollMode);

    // Bindings
    settings.beginGroup("Bindings");
    settings.beginWriteArray("bindings");
    for(int i = 0; i < 7; i++) {
        settings.setArrayIndex(i);
        MouseButton btn = buttons[i];
        settings.setValue("name", btn.name);
        settings.setValue("action", btn.bindedAction);
        settings.setValue("combo", btn.keyboardCombo.toCombined());
        settings.setValue("specialKey", btn.multimediaKey);
    }
    settings.endArray();
    settings.endGroup();

    // DPI
    settings.beginGroup("DPI");
    settings.setValue("apply", applyDpi);
    settings.setValue("active_profile", activeDpi);
    settings.beginWriteArray("profiles");
    for(int i = 0; i < DPI_PROFILES; i++) {
        settings.setArrayIndex(i);
        settings.setValue("value", dpiProfiles[i].dpiValue);
        settings.setValue("enabled", dpiProfiles[i].enabled);
    }
    settings.endArray();
    settings.endGroup();

    settings.beginGroup("Misc");
    settings.setValue("fire_rate", fireRate);
    settings.endGroup();
}

MouseSettings::DPIProfile MouseSettings::getActiveDpiProfile() const {
    return dpiProfiles[activeDpi];
}
