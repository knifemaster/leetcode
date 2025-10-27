#include "settings_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QString>

SettingsDialog::SettingsDialog(float currentConf, float currentNMS, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("YOLO Settings");

    // Sliders and Labels initialization
    confSlider = new QSlider(Qt::Horizontal, this);
    nmsSlider = new QSlider(Qt::Horizontal, this);
    confLabel = new QLabel(this);
    nmsLabel = new QLabel(this);
    okButton = new QPushButton("OK", this);

    // Initial values
    confThreshold = currentConf;
    nmsThreshold = currentNMS;

    // Set slider range (e.g., 0 to 100 for 0.00 to 1.00)
    confSlider->setRange(0, 100);
    nmsSlider->setRange(0, 100);
    
    // Set current values
    confSlider->setValue(static_cast<int>(currentConf * 100));
    nmsSlider->setValue(static_cast<int>(currentNMS * 100));

    // Connect slots
    connect(confSlider, &QSlider::valueChanged, this, &SettingsDialog::updateConfLabel);
    connect(nmsSlider, &QSlider::valueChanged, this, &SettingsDialog::updateNMSLabel);
    connect(okButton, &QPushButton::clicked, this, &SettingsDialog::acceptSettings);

    // Initial display
    updateConfLabel(confSlider->value());
    updateNMSLabel(nmsSlider->value());

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    mainLayout->addWidget(new QLabel("Confidence Threshold:"));
    QHBoxLayout *confLayout = new QHBoxLayout;
    confLayout->addWidget(confSlider);
    confLayout->addWidget(confLabel);
    mainLayout->addLayout(confLayout);

    mainLayout->addWidget(new QLabel("NMS Threshold:"));
    QHBoxLayout *nmsLayout = new QHBoxLayout;
    nmsLayout->addWidget(nmsSlider);
    nmsLayout->addWidget(nmsLabel);
    mainLayout->addLayout(nmsLayout);

    mainLayout->addWidget(okButton);
}

SettingsDialog::~SettingsDialog() {}

void SettingsDialog::updateConfLabel(int value) {
    float val = value / 100.0f;
    confLabel->setText(QString::number(val, 'f', 2));
}

void SettingsDialog::updateNMSLabel(int value) {
    float val = value / 100.0f;
    nmsLabel->setText(QString::number(val, 'f', 2));
}

void SettingsDialog::acceptSettings() {
    confThreshold = confSlider->value() / 100.0f;
    nmsThreshold = nmsSlider->value() / 100.0f;
    accept();
}