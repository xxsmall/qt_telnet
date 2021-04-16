#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/*
 * 这是根据自己兴趣编写的mud客户端，感兴趣的可以自己修改。
 *
 * 作者：  xxsmall@163.com
 *
 * 时间： 2020-03-31
 */
#include <QMainWindow>
#include  <qttelnet.h>
#include  <QDebug>
#include  <QDateTime>
#include  <QScrollBar>
#include  <QMenu>
#include  <QTimer>
#include  <QPixmap>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFileDialog>
#include <QFontDialog>


#include  "bat_name.h"
#include  "dialogeditbat_name.h"
#include  "trigger.h"
#include  "dialogtrigger.h"
#include  "dialogtimer.h"
#include  "uservar.h"
#include  "dialogeditvar.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     QMenu               *menu;        //
     QList<Bat_Name>      batNameList;  //
     QAction             *batNameAction; //
     QAction             *editVarAction; //
     QList<Trigger>       triggerList; //

     QAction             *triggEditAction;//

     QAction             *timerEditAction;//

     QAction             *httpAction;

     QList<UserVar>       userVarList; //用户变量列表提取诗词
     QList<QString>       poemList;

     bool varListIsChanging = false;
     bool batNameListSizeIsChanging = false;
     bool triggerListSizeIsChanging = false;

     QTimer   *timer1;
     int       timer1_Max;
     int       timer1_CountValue;
     bool      timer1_enable;
     QString   timer1_cmd;


     QTimer   *timer2;
     int       timer2_Max;
     int       timer2_CountValue;
     bool      timer2_enable;
     QString   timer2_cmd;

     QNetworkAccessManager *accessManager;

     QString  httpUrl2;

     QList<QString> maxTxtList; //显示文本列表

     void writeFile(QList<QString> textList, QString fileName);
     QList<QString>  readFile(QString fileName);
signals:
    void  processMsg(QString);
    void  sendTimer1_count(int);
    void  sendTimer2_count(int);

private slots:
    void on_pushButton_run_clicked();

    void on_pushButton_close_clicked();

    void telnetLoggedIn();

    void telnetLoginFailed();

    void telnetLoggedOut();

    void telnetConnectionError(QAbstractSocket::SocketError error);

    void telnetMessage(const QString &msg);


    QString stripCR(const QString &msg);

    void lineReturnPressed();

    void showBatNameUi();

    void updateBatNameList(QList<Bat_Name> editList);

    void updateVarList(QList<UserVar> varList);

    void  sendQStringList(QStringList list);  //telnet send cmd list

    void  triggerProcess(QString telNetMsg);  // process trigger

    void showEditTriggerUi();

    void  updateTriggerList(QList<Trigger> editList);

    void  timer1_TimeOut();

    void  timer2_TimeOut();

    void  showEditTimerUi();

    void  showVarUi();

    void  recvTime1_enable(bool enable);
    void  recvTime2_enable(bool enable);
    void  recvTime1_Max(int max);
    void  recvTime2_Max(int max);
    void  recvTime1_cmd(QString cmd);
    void  recvTime2_cmd(QString cmd);

    void  processTimerCMD_send(QString cmdList);

    void finishedSlot(QNetworkReply *reply);

    void on_pushButton_font_clicked();

    void  httpTest();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QtTelnet *t;
    DialogEditBat_Name *batNameUi;
    DialogTrigger      *triggerUi;
    DialogTimer        *timerUi;
    DialogEditVar      *varUi;
};

#endif // MAINWINDOW_H
