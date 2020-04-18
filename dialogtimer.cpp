#include "dialogtimer.h"
#include "ui_dialogtimer.h"

DialogTimer::DialogTimer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTimer)
{
    ui->setupUi(this);
}

DialogTimer::~DialogTimer()
{
    delete ui;
}

void  DialogTimer::recvTimer1_cout(int count)
{
     timer1_CountValue = count;
     ui->spinBox_currentValue->setValue(timer1_CountValue);
}

void  DialogTimer::recvTimer2_cout(int count)
{
     timer2_CountValue = count;
     ui->spinBox_currentValue2->setValue(timer2_CountValue);
}

void   DialogTimer::setTimer1_Max(int max, bool enable)
{
     timer1_Max = max;
     ui->spinBox->setValue(timer1_Max);
     timer1_enable = enable;
     if(timer1_enable)
     {
         ui->pushButton->setText("运行中");
     }else
     {
         ui->pushButton->setText("已停止");
     }
}

void   DialogTimer::setTimer2_Max(int max, bool enable)
{
     timer2_Max = max;
     ui->spinBox_2->setValue(timer2_Max);
     timer2_enable = enable;
     if(timer2_enable)
     {
         ui->pushButton_2->setText("运行中");
     }else
     {
         ui->pushButton_2->setText("已停止");
     }
}

void DialogTimer::on_pushButton_clicked()
{
    if(timer1_enable)
    {
        timer1_enable = false;
        ui->pushButton->setText("已停止");
    }else
    {
        timer1_enable = true;
        ui->pushButton->setText("运行中");
    }
    emit timer1_EnableChange(timer1_enable);

}

void DialogTimer::on_pushButton_2_clicked()
{
    if(timer2_enable)
    {
        timer2_enable = false;
        ui->pushButton_2->setText("已停止");
    }else
    {
        timer2_enable = true;
        ui->pushButton_2->setText("运行中");
    }
    emit timer2_EnableChange(timer2_enable);

}

void DialogTimer::on_spinBox_valueChanged(int arg1)
{
    timer1_Max = arg1;
    emit timer1_MaxChange(timer1_Max);
    qDebug()<<arg1;
}

void DialogTimer::on_spinBox_2_valueChanged(int arg1)
{
    timer2_Max = arg1;
    emit timer2_MaxChange(timer2_Max);

}

void DialogTimer::on_lineEdit_textChanged(const QString &arg1)
{
    QString cmd = arg1;
    emit timer1_CmdChange(cmd);
    qDebug()<<cmd;
}

void DialogTimer::on_lineEdit_2_textChanged(const QString &arg1)
{
    QString cmd = arg1;
    emit timer2_CmdChange(cmd);
    qDebug()<<cmd;
}
