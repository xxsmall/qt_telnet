#ifndef DIALOGEDITVAR_H
#define DIALOGEDITVAR_H

#include <QDialog>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QMessageBox>
#include <QDebug>
#include "uservar.h"

namespace Ui {
class DialogEditVar;
}

class DialogEditVar : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditVar(QWidget *parent = 0);
    ~DialogEditVar();

    QStandardItemModel *model;//table 的model
    int    selectedRow; //表格当前选中那一行，删除用

    QList<UserVar> varList;

    void                setTableHeader(); //设置表格头部文字

    void                updateTableData();    //更新表格数据

signals:
    void  varListChanged(QList<UserVar>);

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_del_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::DialogEditVar *ui;
};

#endif // DIALOGEDITVAR_H
