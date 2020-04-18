#ifndef DIALOGTIMER_H
#define DIALOGTIMER_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class DialogTimer;
}

class DialogTimer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTimer(QWidget *parent = 0);
    ~DialogTimer();

    int       timer1_Max;
    int       timer1_CountValue;
    bool      timer1_enable;



    int       timer2_Max;
    int       timer2_CountValue;
    bool      timer2_enable;
signals:
    void  timer1_MaxChange(int);
    void  timer2_MaxChange(int);
    void  timer1_EnableChange(bool);
    void  timer2_EnableChange(bool);
    void  timer1_CmdChange(QString);
    void  timer2_CmdChange(QString);

public slots:
    void      recvTimer1_cout(int count);
    void      recvTimer2_cout(int count);
    void      setTimer1_Max(int max,bool enable);
    void      setTimer2_Max(int max, bool enable);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_2_textChanged(const QString &arg1);

private:
    Ui::DialogTimer *ui;
};

#endif // DIALOGTIMER_H
