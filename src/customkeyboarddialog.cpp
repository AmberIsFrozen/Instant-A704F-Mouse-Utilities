#include "customkeyboarddialog.h"
#include "data.cpp"
#include "ui_customkeyboarddialog.h"

CustomKeyboardDialog::CustomKeyboardDialog(MouseSettings::MouseButton &btn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomKeyboardDialog),
    button(btn)
{
    ui->setupUi(this);
    ui->radioButton->setChecked(true);

    if(btn.multimediaKey != 0) {
        ui->radioButton_2->setChecked(true);
        ui->specialAction->setCurrentIndex(DataReference::ui_to_special_action.key(btn.multimediaKey, 0));
    } else {
        ui->customKeybind->setKeySequence(QKeySequence(button.keyboardCombo));
    }
}

CustomKeyboardDialog::~CustomKeyboardDialog()
{
    delete ui;
}

void CustomKeyboardDialog::on_radioButton_2_toggled(bool checked)
{
    ui->customKeybind->setEnabled(!checked);
    ui->specialAction->setEnabled(checked);
}


void CustomKeyboardDialog::on_radioButton_toggled(bool checked)
{
    ui->customKeybind->setEnabled(checked);
    ui->specialAction->setEnabled(!checked);
}


void CustomKeyboardDialog::on_specialAction_activated(int index)
{
    button.keyboardCombo = QKeyCombination();
    button.multimediaKey = DataReference::ui_to_special_action.value(index);
}

void CustomKeyboardDialog::on_customKeybind_keySequenceChanged(const QKeySequence &keySequence)
{
    QKeySequence binding = ui->customKeybind->keySequence()[0];

    if(ui->customKeybind->keySequence().count() > 1) {
        // https://bugreports.qt.io/browse/QTBUG-78212
        // First meta key is broken, take the 2nd one.
        if(ui->customKeybind->keySequence()[0].key() == Qt::Key_Super_L || ui->customKeybind->keySequence()[0].key() == Qt::Key_Super_R) {
            binding = ui->customKeybind->keySequence()[1];
        }
    }
    ui->customKeybind->setKeySequence(binding);

    button.multimediaKey = 0x00;
    button.keyboardCombo = binding[0];
}

