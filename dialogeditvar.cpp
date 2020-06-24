#include "dialogeditvar.h"
#include "ui_dialogeditvar.h"

DialogEditVar::DialogEditVar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEditVar)
{
    ui->setupUi(this);
    model =new QStandardItemModel();
    ui->tableView->setModel(model);

}

DialogEditVar::~DialogEditVar()
{
    delete ui;
}

void DialogEditVar::setTableHeader()
{
    QStringList list ;
    list.append("name");

    list.append("type");

    list.append("value");

    model->setHorizontalHeaderLabels(list);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tableView_FileSelect->setColumnWidth(0,300);

}


void  DialogEditVar::updateTableData()  //更新表格数据
{
    model->clear();
    setTableHeader();
    selectedRow = -1;
    int size = varList.size();
    for(int i=0;i<size;i++)
    {
        int j=0;
        QString str = varList[i].varName;
        QStandardItem*  tempItem0 = new QStandardItem(str);
        tempItem0->setBackground(QColor(192,192,192));
        model->setItem(i,j,tempItem0);

        j=j+1;
        str= varList[i].varType;
        QStandardItem*  tempItem1 = new QStandardItem(str);
        tempItem1->setBackground(QColor(192,192,192));
        model->setItem(i,j,tempItem1);

        j=j+1;
        str= varList[i].varValue;
        QStandardItem*  tempItem2 = new QStandardItem(str);
        tempItem2->setBackground(QColor(192,192,192));
        model->setItem(i,j,tempItem2);

    }
}

void DialogEditVar::on_tableView_clicked(const QModelIndex &index)
{
    selectedRow = index.row();
    if(selectedRow>=0 && selectedRow < varList.size())
    {
        ui->lineEdit_VarName->setText(varList[selectedRow].varName);
        ui->lineEdit_VarValue->setText(varList[selectedRow].varValue);
        QString varType = varList[selectedRow].varType;
        if(varType == "double")
        {
            ui->comboBox->setCurrentIndex(0);
        }else if(varType == "string")
        {
            ui->comboBox->setCurrentIndex(1);
        }else if(varType == "bool")
        {
            ui->comboBox->setCurrentIndex(2);
        }
    }
}

void DialogEditVar::on_pushButton_del_clicked()
{
    if(selectedRow>=0 && selectedRow < varList.size())
    {
        varList.removeAt(selectedRow);
        updateTableData();

    }
}

void DialogEditVar::on_pushButton_add_clicked()
{
    UserVar  var;
    var.varName = ui->lineEdit_VarName->text();
    var.varValue = ui->lineEdit_VarValue->text();
    int index = ui->comboBox->currentIndex();
    if(index == 0)
    {
        var.varType = "double";
    }else if(index == 1)
    {
        var.varType = "string";
    }else if(index == 2)
    {
        var.varType = "bool";
    }

    varList.append(var);


    updateTableData();    //更新表格数据
}

void DialogEditVar::on_pushButton_save_clicked()
{
    int size = varList.size();

    if(selectedRow>=0 && selectedRow < size )
    {
        UserVar  var;
        var.varName = ui->lineEdit_VarName->text();
        var.varValue = ui->lineEdit_VarValue->text();
        int index = ui->comboBox->currentIndex();
        if(index == 0)
        {
            var.varType = "double";
        }else if(index == 1)
        {
            var.varType = "string";
        }else if(index == 2)
        {
            var.varType = "bool";
        }

        varList[selectedRow].varName = var.varName;
        varList[selectedRow].varType = var.varType;
        varList[selectedRow].varValue = var.varValue;
    }

    updateTableData();  //更新表格数据



    emit varListChanged(varList);
}

void DialogEditVar::on_pushButton_exit_clicked()
{
    close();
}
