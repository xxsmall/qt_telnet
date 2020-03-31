#ifndef DIALOGEDITBAT_NAME_H
#define DIALOGEDITBAT_NAME_H

#include <QDialog>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QMessageBox>
#include <QDebug>
#include  "bat_name.h"


namespace Ui {
class DialogEditBat_Name;
}

class DialogEditBat_Name : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEditBat_Name(QWidget *parent = 0);
    ~DialogEditBat_Name();

    QList<Bat_Name>     batNameList;  //

    QStandardItemModel *model;//table 的model
    int    selectedRow; //表格当前选中那一行，删除用

    void                setTableHeader(); //设置表格头部文字

    void                updateTableData();    //更新表格数据

signals:
    void  batNameListChanged(QList<Bat_Name>);

private slots:
    void on_pushButton_save_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::DialogEditBat_Name *ui;
};

#endif // DIALOGEDITBAT_NAME_H
