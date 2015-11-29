#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H

#include <QDialog>
#include "ui_globalsettings.h"

class GlobalSettings : public QDialog, public Ui::Dialog
{
    Q_OBJECT
public:
    explicit GlobalSettings(QWidget *parent = 0);
private slots:
    void on_pushButton_clicked();
    void on_tableWidget_cellChanged(int, int);
};

#endif // UI_GLOBALSETTINGS_H

