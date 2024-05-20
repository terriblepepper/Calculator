#include "Calculator.h"
#include"BaseNum.h"
#include"CalDay.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculator* Calculate = new  Calculator();
    Calculator_BaseNum* BaseNum = new Calculator_BaseNum();
    CalDay* DailyCal = new CalDay(); 
    Calculate->setWidgets(BaseNum);
    BaseNum->setWidgets(DailyCal);
    DailyCal->setWidgets(Calculate);
    Calculate->show();
    return a.exec();
}
