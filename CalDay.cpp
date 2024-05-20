#include "CalDay.h"
#include"Calculator.h"
#include"BaseNum.h"
using namespace std;
class Calculator;

CalDay::CalDay(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::CalDayClass())
{
	ui->setupUi(this);
	this->setWindowTitle("Calculator");
	this->setWindowIcon(QIcon(":/iamges/bk.png"));
	init_Ui();
}

CalDay::~CalDay()
{
	delete ui;
}

void CalDay::init_Ui()
{
	auto buttonGroup = new QButtonGroup(this);
	auto btnList = findChildren<QPushButton*>();
	for (auto btn : btnList)
	{
		buttonGroup->addButton(btn);
	}
	Date1 = QDate::currentDate();
	Date2 = QDate::currentDate();
	ui->dateEdit->setDate(Date1);
	ui->dateEdit_2->setDate(Date2);
	ui->label_4->setText("0��");
	//����ť����onButtonClicked()��ϵ�����ĳ����ť�����£���ð�ť��ָ�봫�뺯�����д���0
	connect(buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &CalDay::onButtonGroupClicked);
	connect(ui->dateEdit, &QDateEdit::dateChanged, this, &CalDay::updateDate);
	connect(ui->dateEdit_2, &QDateEdit::dateChanged, this, &CalDay::updateDate);
}

void CalDay::setWidgets(Calculator * w1)
{
	Calculate = w1;
}

void CalDay::updateDate()
{
	Date1 = ui->dateEdit->date();
	Date2 = ui->dateEdit_2->date();
	int during = Date1.daysTo(Date2);
	ui->label_4->setText(QString::number(during) + "��");
}

void CalDay::onButtonGroupClicked(QAbstractButton* btn) 
{
	QString name = btn->text();
	if (name == "SwitchMode") 
	{
		currentPos = this->pos();
		this->hide();
		Calculate->move(currentPos);
		Calculate->show();
	}
}
