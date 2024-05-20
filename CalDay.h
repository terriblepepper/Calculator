#pragma once
#include <QtWidgets/QWidget>
#include <QString>
#include <QWidget>
#include <QButtonGroup>
#include "ui_CalDay.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)    
# pragma execution_character_set("utf-8")    
#endif
QT_BEGIN_NAMESPACE
namespace Ui { class CalDayClass; };
QT_END_NAMESPACE

class Calculator_BaseNum;
class Calculator;
class CalDay : public QWidget
{
	Q_OBJECT

public:
	CalDay(QWidget *parent = nullptr);
	~CalDay();
	void init_Ui();
	void setWidgets(Calculator* w1);//建立切换界面联系
public slots:
	//按钮消息处理
	void onButtonGroupClicked(QAbstractButton* btn);
	void updateDate();
private:
	Ui::CalDayClass* ui;
	QDate Date1, Date2;//用于计算日期的字符串
	Calculator* Calculate;
};
