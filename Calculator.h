#pragma once
#include <QtWidgets/QWidget>
#include <QVector>
#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>
#include<sstream>
#include<cmath>
#include <QButtonGroup>
#include<qdebug.h>
#include"ui_Calculator.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif
QT_BEGIN_NAMESPACE
namespace Ui { class CalculatorClass; };
QT_END_NAMESPACE

extern QPoint currentPos;//记录当前窗口位置
class CalDay;
class Calculator_BaseNum;
class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
    void ini_Ui();
    void setWidgets(Calculator_BaseNum* w2);//建立切换界面联系
public slots:
    void onButtonGroupClicked(QAbstractButton* btn);
    bool btnIsSign(QString btn);
    void Replace(QString* Txt);
    void Eva(Ui::CalculatorClass* ui);
private:
    Ui::CalculatorClass *ui;
    QString preBtn;
    QString Txt,CTxt,ExTxt;
    double result;
    Calculator_BaseNum* CalBase;
};
