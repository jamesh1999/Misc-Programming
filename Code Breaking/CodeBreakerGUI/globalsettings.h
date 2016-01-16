#ifndef __GLOBALSETTINGS_INCLUDED__
#define __GLOBALSETTINGS_INCLUDED__

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

#endif

