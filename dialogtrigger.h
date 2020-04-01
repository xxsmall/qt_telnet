#ifndef DIALOGTRIGGER_H
#define DIALOGTRIGGER_H

#include <QDialog>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QMessageBox>
#include <QDebug>

#include "trigger.h"

namespace Ui {
class DialogTrigger;
}

class DialogTrigger : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTrigger(QWidget *parent = 0);
    ~DialogTrigger();

    QList<Trigger>       triggerEditList; //
    QStandardItemModel *model;//table 的model
    int    selectedRow; //表格当前选中那一行，删除用

    void                setTableHeader(); //设置表格头部文字

    void                updateTableData();    //更新表格数据
signals:
    void  triggerListChanged(QList<Trigger>);
private slots:

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::DialogTrigger *ui;
};

#endif // DIALOGTRIGGER_H
