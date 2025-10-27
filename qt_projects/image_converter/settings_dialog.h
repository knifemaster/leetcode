#ifndef SETTINGS_DIALOG_H
#define SETTINGS_DIALOG_H

#include <QDialog>
#include <QSlider>
#include <QLabel>
#include <QPushButton>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(float currentConf, float currentNMS, QWidget *parent = nullptr);
    ~SettingsDialog() override; 

    float getConfThreshold() const { return confThreshold; }
    float getNMSThreshold() const { return nmsThreshold; }

private slots:
    void updateConfLabel(int value);
    void updateNMSLabel(int value);
    void acceptSettings();

private:
    QSlider *confSlider;
    QSlider *nmsSlider;
    QLabel *confLabel;
    QLabel *nmsLabel;
    QPushButton *okButton;

    float confThreshold;
    float nmsThreshold;
};

#endif // SETTINGS_DIALOG_H