#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    t = new QtTelnet(nullptr);
    ui->textEdit->setReadOnly(true);

    timerUi = new DialogTimer(this);

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

    timerEditAction = new QAction("定时器",this);
    menu->addAction(timerEditAction);
    connect(timerEditAction, SIGNAL(triggered()), this, SLOT(showEditTimerUi()));  //关联槽

    editVarAction = new QAction("变量",this);
    menu->addAction(editVarAction);
    connect(editVarAction, SIGNAL(triggered()), this, SLOT(showVarUi()));  //关联槽


    httpAction = new QAction("http test",this);
    menu->addAction(httpAction);
    connect(httpAction, SIGNAL(triggered()), this, SLOT(httpTest()));  //关联槽

    varUi = new DialogEditVar();
    connect(varUi,SIGNAL(varListChanged(QList<UserVar>)),
            this,SLOT(updateVarList(QList<UserVar>)));


    batNameUi = new  DialogEditBat_Name();

    connect(batNameUi,SIGNAL(batNameListChanged(QList<Bat_Name>)),
            this, SLOT(updateBatNameList(QList<Bat_Name>)));

    connect(this, SIGNAL(processMsg(QString)), this, SLOT(triggerProcess(QString)));  //关联处理触发器槽

    triggerUi = new DialogTrigger();

    connect(triggerUi,SIGNAL(triggerListChanged(QList<Trigger>)),
            this, SLOT(updateTriggerList(QList<Trigger>)));

    connect(this,SIGNAL(sendTimer1_count(int)),
            timerUi, SLOT(recvTimer1_cout(int)));

    connect(this,SIGNAL(sendTimer2_count(int)),
            timerUi, SLOT(recvTimer2_cout(int)));


    connect(timerUi,SIGNAL(timer1_EnableChange(bool)),
            this, SLOT(recvTime1_enable(bool)));

    connect(timerUi,SIGNAL(timer2_EnableChange(bool)),
            this, SLOT(recvTime2_enable(bool)));

    connect(timerUi,SIGNAL( timer1_MaxChange(int)),
            this, SLOT(recvTime1_Max(int)));

    connect(timerUi,SIGNAL( timer2_MaxChange(int)),
            this, SLOT(recvTime2_Max(int)));

    connect(timerUi,SIGNAL( timer1_CmdChange(QString)),
            this, SLOT( recvTime1_cmd(QString)));

    connect(timerUi,SIGNAL( timer2_CmdChange(QString)),
            this, SLOT( recvTime2_cmd(QString)));

    timer1 = new QTimer(this);
    connect(timer1, SIGNAL(timeout()), this, SLOT(timer1_TimeOut()));
    timer1->start(1000); //1000 ms
    timer1_Max =66;
    timer1_CountValue = 60;
    timer1_enable = false;

    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(timer2_TimeOut()));
    timer2->start(1000); //1000 ms
    timer2_Max =66;
    timer2_CountValue = 60;
    timer2_enable = false;

    timerUi->setTimer1_Max(timer1_Max,timer1_enable);
    timerUi->setTimer2_Max(timer2_Max,timer2_enable);

    accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));

    Bat_Name a1;
    a1.nameBat = "a1";
    a1.cmdList =  "i;hp;sc";
    batNameList.append(a1);

    Trigger t2;
    t2.telNetMsg = "要注册新人物请输入new";
    t2.cmdList = "xxsmalltwo";
    t2.enable = true;
    triggerList.append(t2);

    Trigger t3;
    t3.telNetMsg = "此ID档案已存在";
    t3.cmdList = "2222qqqq";
    t3.enable = true;
    triggerList.append(t3);

    Trigger t4;
    t4.telNetMsg = "一家价钱低廉的客栈，生意非常兴隆。外地游客多选择这里落脚";
    t4.cmdList = "a1";
    t4.enable = true;
    triggerList.append(t4);

    QPixmap pixmap("1.jpg");
    ui->label_http->setPixmap(pixmap);

    UserVar v1;
    v1.varName = "bookName";
    v1.varType = "string";
    v1.varValue = "三国演义";
    userVarList.append(v1);

}

MainWindow::~MainWindow()
{
    timer1->stop();
    delete timer1;
    timer1 = NULL;

    timer2->stop();
    delete timer2;
    timer2 = NULL;

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
    if(s->size().height() > 2000)
    {
        ui->textEdit->clear();
    }
    s->setValue(s->maximum());

//    qDebug()<<"++++++++++++++++";
//    qDebug()<<telNetString;
//    qDebug()<<"----------------";

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

void MainWindow::updateVarList(QList<UserVar> varList)
{
    varListIsChanging = true;
    userVarList = varList;
    varListIsChanging = false;
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
    // qDebug()<<"TTTTTT   "<<telNetMsg;
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
         if(telInfoCurrentLine.contains("http://pkuxkx.com/antirobot/robot.php"))
         {
             int pos = telInfoCurrentLine.indexOf("http://pkuxkx.com/antirobot/robot.php");
             httpUrl2 = telInfoCurrentLine.right(telInfoCurrentLine.length()-pos);
             ui->label_http->setText(httpUrl2);
         }

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

void MainWindow:: timer1_TimeOut()
{
   // qDebug()<<"timer 1 runing";
    if(timer1_enable)
    {
        if(timer1_CountValue <=0)
        {
            processTimerCMD_send(timer1_cmd);
            timer1_CountValue = timer1_Max;
        }

        if(timer1_CountValue > timer1_Max)
        {
            timer1_CountValue = 0;
        }

        timer1_CountValue = timer1_CountValue - 1;

        if(timer1_CountValue <0)
        {
            timer1_CountValue = timer1_Max;
        }

        if(timer1_CountValue > timer1_Max)
        {
            timer1_CountValue = 0;
        }

        emit sendTimer1_count(timer1_CountValue);

    }

}

void MainWindow:: timer2_TimeOut()
{
    //qDebug()<<"timer 2 runing";
    if(timer2_enable)
    {
        if(timer2_CountValue <=0)
        {
            processTimerCMD_send(timer2_cmd);
            timer2_CountValue = timer2_Max;
        }

        if(timer2_CountValue > timer2_Max)
        {
            timer2_CountValue = 0;
        }

        timer2_CountValue = timer2_CountValue - 1;

        if(timer2_CountValue <0)
        {
            timer2_CountValue = timer2_Max;
        }

        if(timer2_CountValue > timer2_Max)
        {
            timer2_CountValue = 0;
        }

        emit sendTimer2_count(timer2_CountValue);

    }
}

void  MainWindow::showEditTimerUi()
{
    timerUi->show();
}

void  MainWindow::showVarUi()
{
    varUi->varList = userVarList;
    varUi->updateTableData();
    varUi->show();
}

void  MainWindow::recvTime1_enable(bool enable)
{
     timer1_enable = enable;
}

void  MainWindow::recvTime2_enable(bool enable)
{

    timer2_enable = enable;
}

void   MainWindow::recvTime1_Max(int max)
{
    timer1_Max = max;
}
void   MainWindow::recvTime2_Max(int max)
{
     timer2_Max = max;
}

void  MainWindow::processTimerCMD_send(QString cmdList)
{
    //分割定时器指令队列，以单个指令发送
    QStringList list = cmdList.split(";");
    for(int i=0;i<list.size();i++)
    {
        QString cmd = list[i];
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
            //定时器指令 不在别名队列中 的 处理
            if(t)
            {
                t->sendData(cmd + QString("\r\n"));
            }else
            {
                qDebug()<<"telnet ptr is NULL ! give up send CMD !";
            }
        }
    }
}

void  MainWindow::recvTime1_cmd(QString cmd)
{
    timer1_cmd = cmd;
}

void  MainWindow::recvTime2_cmd(QString cmd)
{
    timer2_cmd = cmd;
}

void MainWindow::finishedSlot(QNetworkReply *reply)
{

   if (reply->error() == QNetworkReply::NoError)
   {
        QByteArray bytes = reply->readAll();
        QString value = bytes;
        qDebug()<<value;

   }
   reply->deleteLater();

}

void MainWindow::httpTest()
{

      QNetworkRequest request;
      QString ip = "http://pkuxkx.com/antirobot/robot.php?filename=1587117811594715";

      QString str1= "line1";
      QString str2= "line2";

      request.setUrl(QUrl(ip));
      request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

      //post
      QNetworkReply* reply = accessManager->get(request);
}

void MainWindow::on_pushButton_font_clicked()
{
        bool ok;
        QFont font = QFontDialog::getFont( &ok, QFont( "Times", 12 ), this );
        if ( ok ) {
            // font被设置为用户选择的字体
            ui->textEdit->setFont(font);
        } else {
            // 用户取消这个对话框，font被设置为初始值，在这里就是Times, 12
        }

}
