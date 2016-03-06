#ifndef COLUMNVIEW_H
#define COLUMNVIEW_H

#include <QDialog>
#include <QString>
#include <vector>

namespace Ui {
class ColumnView;
}

class ColumnView : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnView(QWidget *parent = 0);
    ~ColumnView();
    void setValues(std::vector<QString>);

private slots:
    void on_tableWidget_cellChanged(int row, int column);
    void on_buttonBox_accepted();

signals:
    void returnVector(std::vector<QString>);
private:
    Ui::ColumnView *ui;
    std::vector<QString> available;
    bool enableChanges = false;
};

#endif // COLUMNVIEW_H
