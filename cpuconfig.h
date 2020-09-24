#ifndef CPUCONFIG_H
#define CPUCONFIG_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QPushButton>
#include <QDebug>

#include <QTextEdit>
#include "cpuinfo.h"

#include <unistd.h>


QT_BEGIN_NAMESPACE
namespace Ui { class CPUConfig; }
QT_END_NAMESPACE

class CPUConfig : public QMainWindow
{
    Q_OBJECT

public:
    CPUConfig(QWidget *parent = nullptr);
    ~CPUConfig();

private slots:
    void on_cpuMin_sliderMoved(int position);

    void on_cpuMax_sliderMoved(int position);

    void on_resetButton_clicked();

    void on_applyChanges_clicked();


private:
    Ui::CPUConfig *ui;
    std::vector<CoreInfo> cores;
};
#endif // CPUCONFIG_H
