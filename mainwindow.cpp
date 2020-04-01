#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    t = new QtTelnet(nullptr);
    ui->textEdit->setReadOnly(true);

    connect(t, SIGNAL(loggedIn()),
            this, SLOT(telnetLoggedIn()));

    connect(t, SIGNAL(loginFailed()),
            this, SLOT(telnetLoginFailed()));

    connect(t, SIGNAL(loggedOut()),
            this, SLOT(telnetLoggedOut()));

    connect(t, SIGNAL(connectionError(QAbstractSocket::SocketError)),
            this, SLOT(telnetConnectionError(QAbstractSocket::SocketError)));

    connect(t, SIGNAL(message(const QString &)),
            this, SLOT(telnetMessage(const QString &)));

    connect(ui->lineEdit_input, SIGNAL(returnPressed()),
            this, SLOT(lineReturnPressed()));

    menu  =  menuBar()->addMenu(tr("设置"));
    batNameAction = new QAction("别名",this);

    menu->addAction(batNameAction);
    connect(batNameAction, SIGNAL(triggered()), this, SLOT(showBatNameUi()));  //关联槽

    triggEditAction = new QAction("触发器",this);
    menu->addAction(triggEditAction);
    connect(triggEditAction, SIGNAL(triggered()), this, SLOT(showEditTriggerUi()));  //关联槽



    batNameUi = new  DialogEditBat_Name();

    connect(batNameUi,SIGNAL(batNameListChanged(QList<Bat_Name>)),
            this, SLOT(updateBatNameList(QList<Bat_Name>)));

    connect(this, SIGNAL(processMsg(QString)), this, SLOT(triggerProcess(QString)));  //关联处理触发器槽

    triggerUi = new DialogTrigger();

    connect(triggerUi,SIGNAL(triggerListChanged(QList<Trigger>)),
            this, SLOT(updateTriggerList(QList<Trigger>)));

    Bat_Name a1;
    a1.nameBat = "a1";
    a1.cmdList =  "i;hp;sc";
    batNameList.append(a1);

//    Trigger t2;
//    t2.telNetMsg = "要注册新人物请输入new";
//    t2.cmdList = "xxsmall";
//    triggerList.append(t2);

//    Trigger t3;
//    t3.telNetMsg = "此ID档案已存在";
//    t3.cmdList = "2222qqqq";
//    triggerList.append(t3);

    Trigger t4;
    t4.telNetMsg = "一家价钱低廉的客栈，生意非常兴隆。外地游客多选择这里落脚";
    t4.cmdList = "a1";
    t4.enable = true;
    triggerList.append(t4);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_run_clicked()
{
    QString host = ui->lineEdit_host->text();
    int port = ui->spinBox_port->value();

    if( host.trimmed().length() && port)
    {
        if(t)
        {
            t->connectToHost(host,port);
        }else
        {
            t = new QtTelnet(nullptr);
            t->connectToHost(host,port);
        }
    }


}

void MainWindow::on_pushButton_close_clicked()
{
    if(t)
    {
        t->sendControl(QtTelnet::InterruptProcess);

    }
}



void MainWindow::telnetLoggedIn()
{
    ui->lineEdit_input->setEnabled(true);
   // ui->label_info->setText("Logged in");
    qDebug()<<"Login in";

}

void  MainWindow::telnetLoginFailed()
{
     // ui->label_info->setText("Login failed");
    qDebug()<<"Login failed";
}

void MainWindow::telnetLoggedOut()
{
    ui->lineEdit_input->setEnabled(false);
   // ui->label_info->setText("Logged out");
    qDebug()<<"Logged out";
}


void MainWindow::telnetConnectionError(QAbstractSocket::SocketError error)
{
   // ui->label_info->setText(QString("Connection error: %1").arg(error));
    qDebug()<<QString("Connection error: %1").arg(error);
}

void MainWindow::telnetMessage(const QString &msg)
{
    QString telNetString = stripCR(msg);
    ui->textEdit->append(telNetString);
    QScrollBar *s = ui->textEdit->verticalScrollBar();
    s->setValue(s->maximum());

    qDebug()<<"++++++++++++++++";
    qDebug()<<telNetString;
    qDebug()<<"----------------";

    emit processMsg(telNetString);

}

QString MainWindow::stripCR(const QString &msg)
{
    QString nmsg(msg);
    nmsg.remove('\r');
    nmsg.remove(QRegExp("\033\\[[0-9;]*[A-Za-z]")); // Also remove terminal control codes
    return nmsg;
}


void MainWindow::lineReturnPressed()
{
    QString str = ui->lineEdit_input->text();
   // t->sendData(str+QString("\r\n"));

    bool find_bat_name = false;

    if(batNameListSizeIsChanging)
    {
        sendQStringList(str.split(";"));
        //t->sendData(str+QString("\r\n"));
    }else
    {
        for(int i=0;i<batNameList.size();i++)
        {
            QString tempBatName = batNameList[i].nameBat;
            if(tempBatName == str)
            {
                QString cmdList = batNameList[i].cmdList;
                find_bat_name = true;
                QStringList cmdAll = cmdList.split(";");
                sendQStringList(cmdAll);
                break;
            }
        }


        if(!find_bat_name)
        {
            sendQStringList(str.split(";"));
            //t->sendData(str+QString("\r\n"));
        }
    }

    ui->lineEdit_input->selectAll();

}

void MainWindow::showBatNameUi()
{
     batNameUi->batNameList = this->batNameList;
     batNameUi->updateTableData();
     batNameUi->show();
}

void MainWindow::updateBatNameList(QList<Bat_Name> editList)
{

    batNameListSizeIsChanging  = true;

    this->batNameList = editList;

    batNameListSizeIsChanging = false;

}

void  MainWindow::sendQStringList(QStringList list)
{
    for(int i=0;i<list.size();i++)
    {
        QString tempCMD = list[i];

        t->sendData(tempCMD+QString("\r\n"));

        for(int mm=0;mm<10000;mm++)
        {
            int sleepp =0;
            sleepp = 9;
        }
    }

}

void MainWindow::triggerProcess(QString telNetMsg)
{
     qDebug()<<"TTTTTT   "<<telNetMsg;
     if(triggerListSizeIsChanging)
     {   //编辑好的触发器赋值给mainWindow中的触发器列表时，不进行触发器的匹配
         //否则，会因触发器大小不一致而导致指针越界，程序崩溃
         return ;
     }

     QStringList telInfoList = telNetMsg.trimmed().split("\n");
     //将telnet发来的多行信息，进行分行处理
     for(int m=0;m<telInfoList.size();m++)
     {
         QString telInfoCurrentLine = telInfoList[m];
         telInfoCurrentLine = telInfoCurrentLine.trimmed();

         for(int i=0;i<triggerList.size();i++)
         {
             QString triggerHere = triggerList[i].telNetMsg;
             QString cmd = triggerList[i].cmdList;
             bool enable = triggerList[i].enable;
             if(telInfoCurrentLine.contains(triggerHere) && enable)//触发器目前是包含逻辑
             {
                 bool find_in_batName  = false;
                 for(int j=0;j<batNameList.size();j++)
                 {
                     QString batHere = batNameList[j].nameBat;
                     if(batHere == cmd) //判断当前要发的指令在别名队列中时的处理
                     {
                         find_in_batName = true;
                         QStringList  needSendList =  batNameList[j].cmdList.split(";");
                         sendQStringList(needSendList);
                         break ;
                     }
                 }

                 if(find_in_batName)
                 {
                     qDebug()<<"find bat name and send cmd !";
                 }else
                 {
                     //触发器指令 不在别名队列中 的 处理
                     sendQStringList(cmd.split(";"));
                 }
             }
         }
     }

}

void MainWindow::showEditTriggerUi()
{

    triggerUi->triggerEditList  = this->triggerList;
    triggerUi->updateTableData();
    triggerUi->show();
}

void MainWindow::updateTriggerList(QList<Trigger> editList)
{
    triggerListSizeIsChanging = true;

    this->triggerList = editList;

     triggerListSizeIsChanging = false;

}
