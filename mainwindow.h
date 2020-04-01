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



#include  "bat_name.h"
#include  "dialogeditbat_name.h"
#include  "trigger.h"
#include  "dialogtrigger.h"




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
     QList<Bat_Name>     batNameList;  //
     QAction             *batNameAction; //
     QList<Trigger>       triggerList; //

     QAction             *triggEditAction;//

     bool batNameListSizeIsChanging = false;
     bool triggerListSizeIsChanging = false;
signals:
    void  processMsg(QString);


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

    void  sendQStringList(QStringList list);  //telnet send cmd list

    void  triggerProcess(QString telNetMsg);  // process trigger

    void showEditTriggerUi();

    void  updateTriggerList(QList<Trigger> editList);
private:
    Ui::MainWindow *ui;
    QtTelnet *t;
    DialogEditBat_Name *batNameUi;
    DialogTrigger      *triggerUi;
};

#endif // MAINWINDOW_H
