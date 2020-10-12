#ifndef TRIGGER_H
#define TRIGGER_H

#include <QObject>

class Trigger
{
public:
    explicit Trigger();
    QString  telNetMsg;  //收到的telnetmsg
    QString  cmdList;   //要发送或执行的命令列表



    int      trigerType;//触发方式，上述4中方式之一
    QString  frontTxt;//第一段文本
    QString  variableStore; //要赋值的变量
    QString  behindTxt;//第二段文本
    int      runIfCMDtype ; //（if的运行方式）
    QString  variableName;// （if中的变量名称）
    QString   variableOperation;//（if中变量要进行的操作）
    QString   operationValue; //（if中变量要操作的值）
    //QList<QString>  cmdList; //要发送或执行的命令列表
    bool  haveELse;//是否有else分支
    QList<QString> elseCmdList; //else 分支要执行的命令列表
    bool  enable; //该触发器是否启用

    bool  run(QString telnetMsg); //进行触发器的判读与执行

signals:

public slots:
};

#endif // TRIGGER_H
