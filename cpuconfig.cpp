#include "cpuconfig.h"
#include "ui_cpuconfig.h"

CPUConfig::CPUConfig(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CPUConfig)
{
    ui->setupUi(this);
    QTimer* timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, [=](){
        QTableWidget *CPUStatTable = CPUConfig::findChild<QTableWidget*>("CPUStatTable");
        QTableWidgetItem *avgCPUSpeedCell = CPUStatTable->item(0, 0);
        avgCPUSpeedCell->setText(QString::fromLocal8Bit(CPUInfo::getCurrentCPUFrequency(CPUSpeed::mhz).c_str()));
    });
    timer->start();

    cores = CPUInfo::getProcessorInfo();

    QLabel *cpuPrevilageLabel = CPUConfig::findChild<QLabel*>("rootAccess");
    QPushButton *applyButton = CPUConfig::findChild<QPushButton*>("applyChanges");
    QPushButton *resetButton = CPUConfig::findChild<QPushButton*>("resetButton");

    QTableWidget *CPUStatTable = CPUConfig::findChild<QTableWidget*>("CPUStatTable");
    QTableWidgetItem *allowedMinCPUSpeedCell = CPUStatTable->item(1, 0);
    QTableWidgetItem *allowedMaxCPUSpeedCell = CPUStatTable->item(2, 0);
    QTableWidgetItem *currentMinCPUSpeedCell = CPUStatTable->item(3, 0);
    QTableWidgetItem *currentMaxCPUSpeedCell = CPUStatTable->item(4, 0);
    QTableWidgetItem *cpuCoreCountCell = CPUStatTable->item(5, 0);
    QTableWidgetItem *cpuNameCell = CPUStatTable->item(6, 0);

    QDial *cpuMin = CPUConfig::findChild<QDial*>("cpuMin");
    QDial *cpuMax = CPUConfig::findChild<QDial*>("cpuMax");
    QSpinBox *minimum = CPUConfig::findChild<QSpinBox*>("cpuMinBox");
    QSpinBox *maximum = CPUConfig::findChild<QSpinBox*>("cpuMaxBox");
    cpuMin->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    cpuMax->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    minimum->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    maximum->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);

    // Set some values for the table.
    allowedMinCPUSpeedCell->setText(
        QString::fromLocal8Bit(
            CPUInfo::getMinimumAllowableFrequency(CPUSpeed::mhz).c_str()
        )
    );
    allowedMaxCPUSpeedCell->setText(
        QString::fromLocal8Bit(
            CPUInfo::getMaximumAllowableFrequency(CPUSpeed::mhz).c_str()
        )
    );
    currentMinCPUSpeedCell->setText(
        QString::fromLocal8Bit(
            CPUInfo::getMinimumCurrentFrequency(CPUSpeed::mhz).c_str()
        )
    );
    currentMaxCPUSpeedCell->setText(
        QString::fromLocal8Bit(
            CPUInfo::getMaximumCurrentFrequency(CPUSpeed::mhz).c_str()
        )
    );

    cpuCoreCountCell->setText(
        QString::fromLocal8Bit(
            Utility::convertIntToString(cores.size()).c_str()
        )
    );

    // Get the CPU Model Name
    cpuNameCell->setText(
        QString::fromLocal8Bit(
            cores.at(0).modelName.c_str()
        )
    );

    // This code section here will set the QDial and QSpinbox's minimum value
    // base on the CPU's allowable minimum frequency
    cpuMin->setMinimum(CPUInfo::getMinimumAllowableFrequency() / 1000);
    minimum->setMinimum(CPUInfo::getMinimumAllowableFrequency() / 1000);
    cpuMax->setMinimum(CPUInfo::getMinimumAllowableFrequency() / 1000);
    maximum->setMinimum(CPUInfo::getMinimumAllowableFrequency() / 1000);

    // This code section here is similar to above but is for maximum value
    cpuMin->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    minimum->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    cpuMax->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    maximum->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);

    // This code section here is for setting the current set minimum and
    // maximum frequency.
    cpuMin->setValue(CPUInfo::getMinimumCurrentFrequency() / 1000);
    cpuMax->setValue(CPUInfo::getMaximumCurrentFrequency() / 1000);
    minimum->setValue(CPUInfo::getMinimumCurrentFrequency() / 1000);
    maximum->setValue(CPUInfo::getMaximumCurrentFrequency() / 1000);

    if (getuid()) {
        cpuPrevilageLabel->setText("DISABLED");
        cpuMin->setEnabled(false);
        cpuMax->setEnabled(false);
        minimum->setEnabled(false);
        maximum->setEnabled(false);
        applyButton->setEnabled(false);
        resetButton->setEnabled(false);
        QMessageBox messageBox;
        messageBox.warning(0, "Error","This program is written in a way where you need root access. Otherwise it will not be able to do any system modifications. Program will now disable any functionalities that need root access.");
        messageBox.setFixedSize(500,200);
    }
}

CPUConfig::~CPUConfig()
{
    delete ui;
}


void CPUConfig::on_cpuMin_sliderMoved(int position)
{
    QSpinBox *minimum = CPUConfig::findChild<QSpinBox*>("cpuMinBox");
    QSpinBox *maximum = CPUConfig::findChild<QSpinBox*>("cpuMaxBox");
    QDial *maximumDial = CPUConfig::findChild<QDial*>("cpuMax");
    int maximumValue = maximum->value();

    minimum->setValue(position);
    if (position > maximumValue) {
        maximum->setValue(position);
        maximumDial->setValue(position);
    }
    maximumDial->setMinimum(position);
    maximum->setMinimum(position);
}

void CPUConfig::on_cpuMax_sliderMoved(int position)
{
    QSpinBox *maximum = CPUConfig::findChild<QSpinBox*>("cpuMaxBox");
    maximum->setValue(position);
}

void CPUConfig::on_applyChanges_clicked()
{
    QSpinBox *minimum = CPUConfig::findChild<QSpinBox*>("cpuMinBox");
    QSpinBox *maximum = CPUConfig::findChild<QSpinBox*>("cpuMaxBox");
    QString core_size = QString::fromLocal8Bit(Utility::convertIntToString(cores.size()).c_str());
    //qDebug() << cores.size();
    bool encounteredErrors = false;
    for (size_t i = 0; i < cores.size(); i++) {
        CoreInfo cpuCore = cores.at(i);
        bool setMin = CPUInfo::setMinimumFrequency(cpuCore.processorId, minimum->value(), CPUSpeed::mhz);
        bool setMax = CPUInfo::setMaximumFrequency(cpuCore.processorId, maximum->value(), CPUSpeed::mhz);

        if (!setMin || !setMax) {
            QMessageBox messageBox;
            messageBox.critical(0, "Operation failed","Unable to set CPU frequency, possibly it needs root");
            messageBox.setFixedSize(500,200);
            encounteredErrors = true;
            break;
        }

        //qDebug() << "CPU" << cpuCore.processorId << " set frequency min " << minimum->value() * CPUSpeed::mhz << " frequency max" << maximum->value() * CPUSpeed::mhz;
    }

    if (!encounteredErrors) {
        QTableWidget *CPUStatTable = CPUConfig::findChild<QTableWidget*>("CPUStatTable");
        QTableWidgetItem *currentMinCPUSpeedCell = CPUStatTable->item(3, 0);
        QTableWidgetItem *currentMaxCPUSpeedCell = CPUStatTable->item(4, 0);
        currentMinCPUSpeedCell->setText(
            QString::fromLocal8Bit(
                CPUInfo::getMinimumCurrentFrequency(CPUSpeed::mhz).c_str()
            )
        );
        currentMaxCPUSpeedCell->setText(
            QString::fromLocal8Bit(
                CPUInfo::getMaximumCurrentFrequency(CPUSpeed::mhz).c_str()
            )
        );

        QMessageBox messageBox;
        messageBox.information(0, "Operation success","The changes has been successfully applied to all " + core_size + " cores." );
        messageBox.setFixedSize(500,200);
    }
}

void CPUConfig::on_resetButton_clicked()
{
    QDial *cpuMin = CPUConfig::findChild<QDial*>("cpuMin");
    QDial *cpuMax = CPUConfig::findChild<QDial*>("cpuMax");
    QSpinBox *minimum = CPUConfig::findChild<QSpinBox*>("cpuMinBox");
    QSpinBox *maximum = CPUConfig::findChild<QSpinBox*>("cpuMaxBox");

    cpuMin->setMinimum(CPUInfo::getMinimumAllowableFrequency() / 1000);
    minimum->setMinimum(CPUInfo::getMinimumAllowableFrequency() / 1000);
    cpuMax->setMinimum(CPUInfo::getMinimumAllowableFrequency() / 1000);
    maximum->setMinimum(CPUInfo::getMinimumAllowableFrequency() / 1000);


    cpuMin->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    minimum->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    cpuMax->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);
    maximum->setMaximum(CPUInfo::getMaximumAllowableFrequency() / 1000);

    cpuMin->setValue(CPUInfo::getMinimumCurrentFrequency() / 1000);
    cpuMax->setValue(CPUInfo::getMaximumCurrentFrequency() / 1000);
    minimum->setValue(CPUInfo::getMinimumCurrentFrequency() / 1000);
    maximum->setValue(CPUInfo::getMaximumCurrentFrequency() / 1000);
}

