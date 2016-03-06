#include "ColumnView.h"
#include "ui_ColumnView.h"

ColumnView::ColumnView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColumnView)
{
    ui->setupUi(this);
}

ColumnView::~ColumnView()
{
    delete ui;
}

void ColumnView::setValues(std::vector<QString> values)
{
    enableChanges = false;
    available = values;

    ui->tableWidget->setColumnCount(available.size());
    ui->tableWidget->setRowCount(2);
    QStringList header;
    header << "Index" << "Value";
    ui->tableWidget->setVerticalHeaderLabels(header);

    for(int i = 0; i < available.size(); ++i)
    {
        ui->tableWidget->setItem(0, i, new QTableWidgetItem(QString::number(i)));
        ui->tableWidget->setItem(1, i, new QTableWidgetItem(available[i]));
    }
    enableChanges = true;
}

void ColumnView::on_tableWidget_cellChanged(int row, int column)
{
    if(enableChanges)
    {
        enableChanges = false;

        if(row == 0)
        {
            ui->tableWidget->item(0, column)->setText(QString::number(column));
            enableChanges = true;
            return;
        }

        //If value is allowed
        if(std::find(available.begin(), available.end(), ui->tableWidget->item(row, column)->text()) != available.end())
        {
            QString new_val = ui->tableWidget->item(1, column)->text();
            for(int i = 0; i < available.size(); ++i)
                if(ui->tableWidget->item(1, i)->text() == new_val && column != i)
                {
                    column = i;
                    break;
                }
        }

        //Find the current unused value
        QString unused = "";
        for(auto val : available)
        {
            bool is_used = false;
            for(int i = 0; i < available.size(); ++i)
                if(ui->tableWidget->item(1, i)->text() == val)
                {
                    is_used = true;
                    break;
                }

            if(!is_used)
            {
                unused = val;
                break;
            }
        }
        ui->tableWidget->item(1, column)->setText(unused);

        enableChanges = true;
    }
}

void ColumnView::on_buttonBox_accepted()
{
    std::vector<QString> values;
    for(int i = 0; i < available.size(); ++i)
        values.push_back(ui->tableWidget->item(1, i)->text());
    emit returnVector(values);
}
