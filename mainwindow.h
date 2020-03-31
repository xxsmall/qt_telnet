#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include  <qttelnet.h>
#include  <QDebug>
#include  <QDateTime>
#include  <QScrollBar>
#include  <QMenu>



#include  "bat_name.h"
#include  "dialogeditbat_name.h"




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

     bool batNameListSizeIsChanging = false;


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

private:
    Ui::MainWindow *ui;
    QtTelnet *t;
    DialogEditBat_Name *batNameUi;
};

#endif // MAINWINDOW_H
