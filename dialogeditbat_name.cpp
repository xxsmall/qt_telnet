#include "dialogeditbat_name.h"
#include "ui_dialogeditbat_name.h"

DialogEditBat_Name::DialogEditBat_Name(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditBat_Name)
{
    ui->setupUi(this);
    model =new QStandardItemModel();
    ui->tableView->setModel(model);

}

DialogEditBat_Name::~DialogEditBat_Name()
{
    delete ui;
}

void DialogEditBat_Name::on_pushButton_save_clicked()
{


    int size = batNameList.size();

    if(selectedRow>=0 && selectedRow < size )
    {
      batNameList[selectedRow].nameBat =  ui->lineEdit_batName->text();
      batNameList[selectedRow].cmdList =  ui->lineEdit_cmdList->text();
    }

    updateTableData();  //更新表格数据

    for(int i=0;i<size;i++)
    {
        qDebug()<<"=====================";
        qDebug()<<batNameList[i].nameBat;
        qDebug()<<batNameList[i].cmdList;
        qDebug()<<batNameList[i].enable;
        qDebug()<<"---------------------";
    }

    emit batNameListChanged(batNameList);
}

void   DialogEditBat_Name::setTableHeader()
{
    QStringList list ;
    list.append("name");

    list.append("stringList");

    model->setHorizontalHeaderLabels(list);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tableView_FileSelect->setColumnWidth(0,300);

}

void  DialogEditBat_Name::updateTableData()  //更新表格数据
{
    model->clear();
    setTableHeader();
    selectedRow = -1;
    int size = batNameList.size();
    for(int i=0;i<size;i++)
    {
        int j=0;
        QString str = batNameList[i].nameBat;
        QStandardItem*  tempItem0 = new QStandardItem(str);
        tempItem0->setBackground(QColor(192,192,192));
        if(batNameList[i].enable)
        {
            tempItem0->setCheckable(true);
        }else
        {
            tempItem0->setCheckable(false);
        }
        model->setItem(i,j,tempItem0);

        j=j+1;
        str= batNameList[i].cmdList;
        QStandardItem*  tempItem1 = new QStandardItem(str);
        tempItem1->setBackground(QColor(192,192,192));
        model->setItem(i,j,tempItem1);
    }
}

void DialogEditBat_Name::on_tableView_clicked(const QModelIndex &index)
{
    selectedRow = index.row();
    if(selectedRow>=0 && selectedRow < batNameList.size())
    {
        ui->lineEdit_batName->setText(batNameList[selectedRow].nameBat);
        ui->lineEdit_cmdList->setText(batNameList[selectedRow].cmdList);
    }
}

void DialogEditBat_Name::on_pushButton_add_clicked()
{
    Bat_Name  bat;
    bat.nameBat = ui->lineEdit_batName->text();
    bat.cmdList = ui->lineEdit_cmdList->text();
    batNameList.append(bat);
    updateTableData();    //更新表格数据

}

void DialogEditBat_Name::on_pushButton_del_clicked()
{
    if(selectedRow>=0 && selectedRow < batNameList.size())
    {
        batNameList.removeAt(selectedRow);
        updateTableData();

    }
}

void DialogEditBat_Name::on_pushButton_exit_clicked()
{
    close();
}
