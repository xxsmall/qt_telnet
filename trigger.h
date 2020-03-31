#ifndef TRIGGER_H
#define TRIGGER_H

#include <QObject>

class Trigger
{
public:
    explicit Trigger();
    QString  telNetMsg;
    QString  cmdList;
    bool     enable;

signals:

public slots:
};

#endif // TRIGGER_H
