#ifndef BAT_NAME_H
#define BAT_NAME_H
#include <QString>
#include <stdio.h>
using namespace  std;

class Bat_Name
{
public:
    Bat_Name();
public:
    QString  nameBat;
    QString  cmdList;
    bool     enable;
};

#endif // BAT_NAME_H
