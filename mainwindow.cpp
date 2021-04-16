#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    t = new QtTelnet(0);
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

    poemList.clear();//

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

    Bat_Name a2;
    a2.nameBat = "adg";
    a2.cmdList =  "ask daoguo about 新手任务";
    batNameList.append(a2);

    Bat_Name a3;
    a3.nameBat = "adgf";
    a3.cmdList =  "ask daoguo about cancel";
    batNameList.append(a3);

    Bat_Name a4;
    a4.nameBat = "hb"; //显示可接的任务列表
    a4.cmdList = "listesc";
    batNameList.append(a4);

    Bat_Name a5;
    a5.nameBat = "jb";
    a5.cmdList = "getesc"; //接任务的命令
    batNameList.append(a5);

    Bat_Name a6;
    a6.nameBat = "music";
    a6.cmdList = "learn huang for music::music-theory 29"; //学音乐的命令
    batNameList.append(a6);

    Bat_Name a7;
    a7.nameBat = "skillsSub";
    a7.cmdList = "skills -sub music huang"; //接任务的命令
    batNameList.append(a7);
    //一叶扁舟缓缓地驶了过来，艄公将一块踏脚板搭上堤岸，
    Trigger t1;
    t1.telNetMsg = "一叶扁舟缓缓地驶了过来，艄公将一块踏脚板搭上堤岸";
    t1.cmdList = "enter";
    t1.enable = true;
    triggerList.append(t1);

    //正等着你呢，上来吧

    Trigger t1a;
    t1a.telNetMsg = "正等着你呢，上来吧";
    t1a.cmdList = "enter";
    t1a.enable = true;
    triggerList.append(t1a);


    Trigger t2;
    t2.telNetMsg = "要注册新人物请输入new";
    t2.cmdList = "xxsmall";
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

    Trigger t5;
    t5.telNetMsg = "一，生意非常兴隆。外地游客多选择这里落脚";
    t5.cmdList = "bbbbbb";
    t5.enable = true;
    triggerList.append(t5);


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
            t = new QtTelnet(0);
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
    //ui->lineEdit_input->setEnabled(true);
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
   // ui->lineEdit_input->setEnabled(false);
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
    int size = maxTxtList.size();
    if(size < 100)
    {
        maxTxtList.append(telNetString);
    }else
    {
        maxTxtList.removeAt(0);
        maxTxtList.append(telNetString);
    }

    QString tempStr;
    for(int i=0 ; i<maxTxtList.size();i++)
    {
        tempStr= tempStr + maxTxtList[i];
    }

    //ui->textEdit->append(telNetString);
    ui->textEdit->setText(tempStr);

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
         //
         //茶博士点头道：“闻道黄龙戍  频年不解兵”不错！不错！
         QString strKey1= "茶博士点头道：";
          QString strKey2= "不错！不错！";
         if(telInfoCurrentLine.contains(strKey1) && telInfoCurrentLine.contains(strKey2))
         {
             QString answer = telInfoCurrentLine.replace(strKey1,"");
             answer =  answer.replace(strKey2,"");
             answer = answer.replace("“","");
             answer = answer.replace("”","");
             answer = answer.replace("  ","");
             poemList.clear();
             poemList = readFile("poem.txt");
             int size = poemList.size();
             bool find = false;
             for(int i=0;i<size;i++)
             {
                 if(poemList.at(i) == answer)
                 {
                     find = true;
                     break;
                 }
             }

             if(find)
             {

             }else
             {
                  poemList.append(answer);
                  int size2 = poemList.size();
                  for(int i=0;i<size2;i++)
                  {
                      poemList[i] = poemList[i].trimmed() + "\r\n";
                  }
                  writeFile(poemList,"poem.txt");
             }

             ui->label_http->setText(telInfoCurrentLine+"\r\n"+answer);
         }

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

/*
最近在论坛上貌似看到很多新人在问少林新手任务的问题，下面我就来详细讲解下少林新手任务，以及做全自动少林新手任务的思路（最近太忙，没时间做，只列出思路,各人自行琢磨）
少林新手任务目前分为六块
劈材，挑水，敲钟，击鼓，诵经，超度，再道果禅师那接任务（ask daoguo about 新手任务）他就会随机给你一项任务。

劈材：劈材就是到少林斋房后面，，然后有和尚会告诉你要几寸宽的哪种材多少个（目前有松材，柏材）同时会给你一把柴刀你只需要wield dao然后take chai，这样你就会随机取到一种原料，然后put 原料id 再根据任务要求的尺寸pi 1-3，就可得到木材，但是得到的木材不确定，有时候不是任务要求你的，所以需要多pi几次，得到任务规定的数量尺寸的材料之后，jiao wood 直到交完，然后jiao dao，就可以回去找道果ask daoguo about success，就完成了任务（此任务增加基本刀法和剑法）
机器人思路：目前有样子：粗糙的（rough），大段的（large），光滑的（smooth），弯曲的（bending），小段的（small），直的（straight）部位：树杈（crotch），树干（trunk），树根（root），树枝（branch），因此这边可以做两个变量，一个是样子，一个是部位，意思就是说，比如我做变量a和b a存放样子，b存放部位，不管我拿的是哪种原料，都直接put @a @b 就行了，这样就可实现，自动取原料，自动放原料，你只管劈就行了，做劈材机器人的难点也就是这里，劈材中别的过程我就不细说了，那些都应该是基本了。

挑水：挑水也是到少林斋房后面，直接carry mu tong，然后到少林山下的佛心井fill mu tong然后再回到斋房后院，fill shui gang，直到提示水缸满就行了，要注意回来的时候要一步一步走，走太快会将水洒掉，白跑一趟（此任务增加基本轻功）
机器人思路：最简单的任务，回来时候做好间隔就行了

敲钟：需要到钟楼七层，xuqi start 然后根据颜色提示（注意，这任务不是看文字描述的，看字体颜色提示，当字体开始闪动了，不管描述的文字是啥，都直接knock bell）然后达到任务要求的次数，xuqi end，此时不要急于回去，等出现文字说蓄气结束了，再过几秒出去，过急的话，有busy，很有可能会导致直接从7层摔出去摔死。（此任务增加基本内功）
机器人思路：要会做颜色触发，然后把每种描述的文字闪烁都做成触发（因为他文字闪烁时描述的是哪句话都有可能）。

击鼓：同敲钟，只是要去鼓楼七层，hit drum，其他同敲钟

诵经：诵经的话，需要你找到藏经楼一楼的HCC NPC jie 任务要求的经书 然后到相应的地点，page 页数 chanting 页数 内容
很多人不知道诵经任务怎么做，或者经书应该怎么看，往往都盲目的一页一页试，这样比较麻烦其实经书的话，
分为4个部分 左上角 XXXXX品第N（XX）这就像一本书的第几章第几节一样，任务要求的一半也就是从这边来分辨，
比如，任务要求你诵经阿含经的，中阿含七法品城喻经第三(16)这个第三就是指阿含经的第三章这个16，
并不是指阿含经的第16页，而是指阿含经的第三章的第16节，而一本经书，也是以第一，第二，第三，这样以此类推。
中间是正文部分不多说，反正看不懂。左下角，是经书名，右下角，是页数，因此，当任务要求你去比如诵经
中阿含七法品城喻经第三(16)，你需要先根据左上角找到，第三（16）就是第三章16节，然后再看右下角
，这个节所在的页数，然后chanting 页数 正文（字与字之间不带空格，标点符号，和====如果直接复制正文的话，
需要把所有的空格和===都去掉，注意不要把字给删掉）然后你只要等系统一秒一个字的慢慢把 经文内容全部输出来之后，
任务就完成了。
机器人思路，和做红豆一样，你需要先把9本经书的内容都事先做好记录，然后根据任务要求，从中节选（工作量太大，一本书正常的话三四百节）

超度：和诵经差不多，只是不在少林寺内，而且任务开始也不告诉你要哪本书，你需要接到任务后，先到任务地点，然后找到高僧（gao seng）,give letter to gao seng;ask seng about 法事;
然后他就会带你进灵堂，release start;之后你就可以知道是哪本书，再稍等下，就可以知道是第几章第几节，然后ask seng about end;这样就可以出灵堂，你回去去借书，然后再回来，ask seng about 法事，
就可以再进到灵堂，翻到任务要求的那一节往后一到两节，然后对一下内容，如果是别人正在念的那一页，赶快pagedown到下一页事先打好release chanting 页数（不用加内容） 不敲回车等待，系统提示又翻了一页，就马上敲回车，
这样就可以配合别人的节奏一起超度了，当一页念完就可以ask seng about end ,然后回去交任务。身上同时带几本书是不明智的，因为没有优先级，所以不管你输入什么命令，都是从你身上的第一本书的第一页开始翻起。
（超度和诵经都增加禅宗心法）

机器人思路：除了跟诵经机器人一样，需要有9本经书的文字库以外还需要会自动走路，因为天南地北都有可能分配到。

最近挺忙，没时间写这个机器人，因此就把和攻略一起把思路放出来，有空的，有想试试的，可自行尝试。

少林派
少林新手任务目前分为六块 劈柴，挑水，敲钟，击鼓，诵经，超度，再道果禅师那接任务（ask daoguo about 新手任务）他就会随机给你一项任务。
具体制作过程如下：
劈柴
领到任务后 n n w n n w ask seng about 劈柴，ask seng about 柴刀，wield dao,take chai,put 柴id（柴不同，id不同），pi 1（表示劈成一寸的柴，如果是2就是二寸的），用jiao命令上缴所需木柴，给够一定数量后，jiao dao把柴刀也还了，就可以回去领奖励。ask chanshi about success。加少量基本剑法或刀法。
劈柴就是到少林斋房后面，ask seng about 劈柴，ask seng about 柴刀，然后中年僧人(Seng ren)会告诉你要几寸宽的哪种柴多少个（目前有松柴，柏柴）同时会给你一把柴刀你只需要wield dao然后take chai，这样你就会随机取到一种原料，然后put 原料id 再根据任务要求的尺寸pi 1-3，就可得到木柴，但是得到的木柴不确定，有时候不是任务要求你的，所以需要多pi几次，得到任务规定的数量尺寸的材料之后，jiao wood 直到交完，然后jiao dao，就可以回去找道果ask daoguo about success，就完成了任务（此任务增加基本刀法和剑法）
机器人思路：(目前有的)
样子：
粗糙的（rough）
大段的（large）
光滑的（smooth）
弯曲的（bending）
小段的（small）
直的（straight）
部位：
树杈（crotch）
树干（trunk）
树根（root）
树枝（branch）
因此这边可以做两个变量，一个是样子，一个是部位，意思就是说，比如我做变量a和b a存放样子，b存放部位，不管我拿的是哪种原料，都直接put @a @b 就行了，这样就可实现，自动取原料，自动放原料，你只管劈就行了，做劈柴机器人的难点也就是这里，劈柴中别的过程我就不细说了，那些都应该是基本了。
挑水
也是去厨房 n n w n n w carry tong后到山腰佛心井，fill tong，后慢慢走回厨房。fill gang，重复几次后水缸满了就可以回去chanshi那ask about success。加基本轻功。
挑水也是到少林斋房后面，直接carry mu tong，然后到少林山下的佛心井fill mu tong然后再回到斋房后院，fill shui gang，直到提示水缸满就行了，要注意回来的时候要一步一步走，走太快会将水洒掉，白跑一趟（此任务增加基本轻功）
机器人思路：最简单的任务，回来时候做好间隔就行了
敲钟/击鼓
到钟楼（n;e;enter）或鼓楼(n;w;enter)顶层7楼后，xuqi start，等到提示语句闪烁的时候就knock bell（hit drum），敲到一定次数后，就会提示该回去报告了，xuqi end后就回去chanshi那里ask about success。加基本内功。
需要到钟楼七层，xuqi start 然后根据颜色提示（注意，这任务不是看文字描述的，看字体颜色提示，当字体开始闪动了，不管描述的文字是啥，都直接knock bell）然后达到任务要求的次数，xuqi end，此时不要急于回去，等出现文字说蓄气结束了，再过几秒出去，过急的话，有busy，很有可能会导致直接从7层摔出去摔死。（此任务增加基本内功）
机器人思路：要会做颜色触发，然后把每种描述的文字闪烁都做成触发（因为他文字闪烁时描述的是哪句话都有可能）。
诵经
去他提示的地方，什么罗汉堂x部，等，打开书，page 1（1表示整本书的第一页，）page到提示的那一章的那一分页后，chanting 页数 内容（例如chanting 10 xxxxxxxxxxxxxxxxxx 就是那一页的内容都复制进去，连起来，不要==这些符号），书没有可以去藏经阁hcc npc那里jie，大幅加禅宗心法。
诵经的话，需要你找到藏经楼一楼的HCC NPC jie 任务要求的经书 然后到相应的地点，page 页数 chanting 页数 内容很多人不知道诵经任务怎么做，或者经书应该怎么看，往往都盲目的一页一页试，这样比较麻烦其实经书的话，分为4个部分 左上角 XXXXX品第N（XX）这就像一本书的第几章第几节一样，任务要求的一半也就是从这边来分辨，比如，任务要求你诵经阿含经的，中阿含七法品城喻经第三(16)这个第三就是指阿含经的第三章这个16，并不是指阿含经的第16页，而是指阿含经的第三章的第16节，而一本经书，也是以第一，第二，第三，这样以此类推。中间是正文部分不多说，反正看不懂。左下角，是经书名，右下角，是页数，因此，当任务要求你去比如诵经 中阿含七法品城喻经第三(16)，你需要先根据左上角找到，第三（16）就是第三章16节，然后再看右下角，这个节所在的页数，然后chanting 页数 正文（字与字之间不带空格，标点符号，和====如果直接复制正文的话，需要把所有的空格和===都去掉，注意不要把字给删掉）然后你只要等系统一秒一个字的慢慢把 经文内容全部输出来之后，任务就完成了。
机器人思路，和做红豆一样，你需要先把9本经书的内容都事先做好记录，然后根据任务要求，从中节选（工作量太大，一本书正常的话三四百节）
超度
去到提示那个地方找到高僧，give letter to gao seng,ask seng about 法事，进去后release start会提示在念哪本经书，
然后ask seng about 离开，回去找到这本书，再回去，ask seng about 法事,进去后release check，会提示你要念该经书的前中或后部，
然后page查看该经书有几页，再release check 页数,会提示你猜的页码是否够接近了，接近程度红>黄>绿。page 到正在诵经的那页，
等开始下一页的时候，马上release chanting 页数.等读满一页后，release end.可以回去领奖了。有个窍门，诵经每翻一页都会有提示，
记住该页开头的几个字，在接近的page逐页校对开头的字是否一致。
和诵经差不多，只是不在少林寺内，而且任务开始也不告诉你要哪本书，你需要接到任务后，先到任务地点，然后找到高僧（gao seng）,
give letter to gao seng;ask seng about 法事;然后他就会带你进灵堂，release start;之后你就可以知道是哪本书，再稍等下
，就可以知道是第几章第几节，然后ask seng about 离开;这样就可以出灵堂，你回去去借书，然后再回来，ask seng about 法事，
就可以再进到灵堂，翻到任务要求的那一节往后一到两节，然后对一下内容，如果是别人正在念的那一页，
赶快pagedown到下一页事先打好release chanting 页数（不用加内容） 不敲回车等待，系统提示又翻了一页，
就马上敲回车，这样就可以配合别人的节奏一起超度了，当一页念完就可以release end;ask seng about 离开;然后回去交任务。
身上同时带几本书是不明智的，因为没有优先级，所以不管你输入什么命令，都是从你身上的第一本书的第一页开始翻起。
（超度和诵经都增加禅宗心法）
机器人思路：除了跟诵经机器人一样，需要有9本经书的文字库以外还需要会自动走路，因为天南地北都有可能分配到。
超度bug，说了让去超度，但没有seng在的地点有：苏州狮子林，长安小雁塔
注：佛心井在少林半山腰
【大侠】江湖再也没有上官隐(317679580) 2020-11-18 11:44:13
set public 1;finger 你的ID

【大侠】江湖再也没有上官隐(317679580) 2020-11-18 11:44:28
注册时用的邮箱和验证时的邮箱没有关系

 【求助】金轮(Falun): ask 天地会
【求助】大高手告诉金轮,天地会暗道:"天地会暗道在棺材铺，knock guancai 3,ed, 想去找陈近南的话：do 3 s,w,e,do 4 n。负道德的小心这厮发飙。
出去的路径：do 4 s,w,n,n,e,n,s,wu"[(becool)提供]

襄阳-南门(ask jiang about 投军)
襄阳-郭府ask nvyong about 郭大侠得纸条
襄阳-帅府中ask guo about 报国，去北门(give ling to jiang)，然后一直往北走。（这一步可以省略）（这里导游图中需 ride 马过草原 ，实测可直接走）
蒙古营门answer 送信，kill 蒙哥（菜鸟用hit到晕再kill，最好去荣宝斋买点好装备去杀或者喊人帮忙），get fu，回襄阳帅府把兵符给郭靖give fu to guo。
可分别读blade和sword至180级。80级skills打不过守门的，见不到虚竹。当铺里的虚竹也可以用，不过那不是正常流程。阅读方式是read tianlong xxx，比如read tianlong 5

去灵鹫宫见虚竹，ask xu about 梦姑；
去 灵州公主寝宫 ，回答门卫answer 送信；
公主，ask princess about 梦郎，拿到一封信；
回去找虚竹，give letter to xu；
把虚竹带到西夏皇宫见公主，他们会一起离开；
再去灵鹫宫，ask xu about 天龙八部 ，即可拿到
*/

void MainWindow::on_pushButton_clicked()
{
    maxTxtList.clear();
}

void MainWindow::writeFile(QList<QString> textList, QString fileName)
{
    //writetext
    if(textList.size()>0)
    {
       QFile file(fileName);
       bool ok;
       //ok=file.open( QIODevice::ReadWrite | QIODevice::Append);
       if(file.exists())
       {
           if(file.isOpen())
           {
               file.close();
           }
           file.remove();
       }
       ok=file.open( QIODevice::ReadWrite );
       if(ok)
       {
           QTextStream out(&file);

           for(int i=0;i<textList.size();i++)
           {
               out << textList[i];
           }

           file.close();
          // QMessageBox::information(this,"info",fileName+" create success!");
       }else
       {
          //  QMessageBox::information(this,"info","can not open file");
       }
    }else
    {
      //  QMessageBox::information(this,"info","no data need to write");
    }
}


QList<QString>  MainWindow::readFile(QString fileName)
{
/*
    QFile file(fileName);
    QList<QString> dataList;
    dataList.clear();

    if(file.isOpen())
    {
        file.close();
    }

    if(!file.open(QIODevice::ReadOnly))
    {
         return dataList;
    }
    QTextStream in(&file);

    int i=0;
    while(!in.atEnd())
    {
        QString onecount = in.readLine();
        i=i+1;
        onecount = onecount.trimmed();
        if(onecount.length() > 0)
        {
            dataList.append(onecount);
        }
        //qDebug() << i <<"  "<< onecount;
    }
    file.close();
    return dataList;
*/

    QFile file(fileName);
    QList<QString> dataList;
    dataList.clear();

    if(file.isOpen())
    {
        file.close();
    }

    if(!file.open(QIODevice::ReadOnly))
    {
         return dataList;
    }
    QTextStream in(&file);

    int i=0;
    while(!in.atEnd())
    {
        QString strAll = in.readAll();
        dataList = strAll.split("\r");

        //qDebug() << i <<"  "<< onecount;
    }

    QList<QString> dataList2;
    for(int i=0;i<dataList.size();i++)
    {
        QString temp = dataList[i];
        temp = temp.trimmed();
        if(temp.size() > 1)
        {
            dataList2.append(temp);
        }
    }
    file.close();
    return dataList2;

}
