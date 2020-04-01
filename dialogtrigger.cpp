#include "dialogtrigger.h"
#include "ui_dialogtrigger.h"

DialogTrigger::DialogTrigger(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTrigger)
{
    ui->setupUi(this);
    model =new QStandardItemModel();
    ui->tableView->setModel(model);
}

DialogTrigger::~DialogTrigger()
{
    delete ui;
}

void   DialogTrigger::setTableHeader()
{
    QStringList list ;
    list.append(" trigger name");

    list.append("send command");

    list.append("enable");

    model->setHorizontalHeaderLabels(list);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tableView_FileSelect->setColumnWidth(0,300);

}

void  DialogTrigger::updateTableData()  //更新表格数据
{
    model->clear();
    setTableHeader();
    selectedRow = -1;
    int size = triggerEditList.size();
    for(int i=0;i<size;i++)
    {
        int j=0;
        QString str = triggerEditList[i].telNetMsg;
        QStandardItem*  tempItem0 = new QStandardItem(str);
        tempItem0->setBackground(QColor(192,192,192));

        model->setItem(i,j,tempItem0);

        j=j+1;
        str= triggerEditList[i].cmdList;
        QStandardItem*  tempItem1 = new QStandardItem(str);
        tempItem1->setBackground(QColor(192,192,192));
        model->setItem(i,j,tempItem1);

        j=j+1;
        if(triggerEditList[i].enable)
        {
            str = "√";
        }else
        {
            str = "×";
        }
        QStandardItem*  tempItem2 = new QStandardItem(str);
        tempItem2->setBackground(QColor(192,192,192));
        model->setItem(i,j,tempItem2);
    }
}


void DialogTrigger::on_tableView_clicked(const QModelIndex &index)
{
    selectedRow = index.row();
    if(selectedRow>=0 && selectedRow < triggerEditList.size())
    {
        ui->lineEdit_triggerTxt->setText(triggerEditList[selectedRow].telNetMsg);
        ui->lineEdit_sendCommandList->setText(triggerEditList[selectedRow].cmdList);
        ui->checkBox_enable->setChecked(triggerEditList[selectedRow].enable);
    }
}

void DialogTrigger::on_pushButton_add_clicked()
{
     Trigger tri;
     tri.enable = ui->checkBox_enable->isChecked();
     tri.telNetMsg = ui->lineEdit_triggerTxt->text();
     tri.cmdList = ui->lineEdit_sendCommandList->text();
     triggerEditList.append(tri);
     updateTableData();
}

void DialogTrigger::on_pushButton_del_clicked()
{
    if(selectedRow>=0 && selectedRow < triggerEditList.size())
    {
        triggerEditList.removeAt(selectedRow);
        updateTableData();

    }
}

void DialogTrigger::on_pushButton_save_clicked()
{
    int size = triggerEditList.size();

    if(selectedRow>=0 && selectedRow < size )
    {
      triggerEditList[selectedRow].telNetMsg =  ui->lineEdit_triggerTxt->text();
      triggerEditList[selectedRow].cmdList =  ui->lineEdit_sendCommandList->text();
      triggerEditList[selectedRow].enable = ui->checkBox_enable->isChecked();
    }

    updateTableData();  //更新表格数据

    for(int i=0;i<size;i++)
    {
        qDebug()<<"=====================";
        qDebug()<<triggerEditList[i].telNetMsg;
        qDebug()<<triggerEditList[i].cmdList;
        qDebug()<<triggerEditList[i].enable;
        qDebug()<<"---------------------";
    }

    emit triggerListChanged(triggerEditList);
}
