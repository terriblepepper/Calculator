#pragma once
#include <QtWidgets/QWidget>
#include <QString>
#include<string>
#include <QButtonGroup>
#include<qdebug.h>
#include <QChar>
#include<math.h>
#include"ui_BaseNum.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif
QT_BEGIN_NAMESPACE
namespace Ui { class Calculator_BaseNumClass; };
QT_END_NAMESPACE

class Calculator;
class CalDay;
class Calculator_BaseNum : public QWidget
{
    Q_OBJECT

public:
    //初始化ui
    Calculator_BaseNum(QWidget *parent = nullptr);
    ~Calculator_BaseNum();   
    void init_Ui();

    //进制转换
    void conversion(int pos);
    void _10toN(int pos);
    void _Nto10(int src);
    void _2toN(int pos);
    void _8toN(int pos);
    void _16toN(int pos);
    void setWidgets(CalDay* w3);//建立切换界面联系
public slots:
    //按钮消息处理
    void onButtonGroupClicked(QAbstractButton* btn);
   
private:
    Ui::Calculator_BaseNumClass *ui;
    QString temp, Value;//用于进制转换的字符串
    int type;//定义进制类型
    char Dec;//_10toN()的小数位变量
    CalDay* Caldaily;
};



