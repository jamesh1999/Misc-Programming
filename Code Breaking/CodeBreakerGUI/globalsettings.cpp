#include "GlobalSettings.h"
#include "ui_GlobalSettings.h"
#include "Globals.h"

GlobalSettings::GlobalSettings(QWidget *parent) : QDialog(parent)
{
    setupUi(this);

    tableWidget->item(0, 1)->setText(QString::number(ENGLISH_IC));
    tableWidget->item(1, 1)->setText(QString::number(ENGLISH_IC_DIFFERENCE));
    tableWidget->item(2, 1)->setText(QString::number(TEMP));
    tableWidget->item(3, 1)->setText(QString::number(STEP));
    tableWidget->item(4, 1)->setText(QString::number(COUNT));
    tableWidget->item(5, 1)->setText(QString::number(LENGTH));
    pushButton->setEnabled(false);
}

void GlobalSettings::on_pushButton_clicked()
{
    ENGLISH_IC = tableWidget->item(0, 1)->text().toFloat();
    ENGLISH_IC_DIFFERENCE = tableWidget->item(1, 1)->text().toFloat();
    TEMP = tableWidget->item(2, 1)->text().toFloat();
    STEP = tableWidget->item(3, 1)->text().toFloat();
    COUNT = tableWidget->item(4, 1)->text().toInt();
    LENGTH = tableWidget->item(5, 1)->text().toInt();

    pushButton->setEnabled(false);
}

void GlobalSettings::on_tableWidget_cellChanged(int, int)
{
    pushButton->setEnabled(true);
}
