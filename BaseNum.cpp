#include"CalDay.h"
#include "BaseNum.h"
#include"Calculator.h"

using namespace std;
Calculator_BaseNum::Calculator_BaseNum(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calculator_BaseNumClass())
{
    ui->setupUi(this);
    this->setWindowTitle("Calculator");
    this->setWindowIcon(QIcon(":/iamges/bk.png"));
    init_Ui();
}

Calculator_BaseNum::~Calculator_BaseNum()
{
    delete ui;
}

//初始化ui
void Calculator_BaseNum::init_Ui() 
{
    auto buttonGroup = new QButtonGroup(this);
    auto btnList = findChildren<QPushButton*>();
    for (auto btn : btnList)
    {
        buttonGroup->addButton(btn);
    }
    //将按钮组与onButtonClicked()联系，如果某个按钮被按下，则该按钮的指针传入函数进行处理0
    connect(buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &Calculator_BaseNum::onButtonGroupClicked);
    type = 10;
    ui->Label->setText("Dec:");
}

/*进制转换*/
void Calculator_BaseNum::_10toN(int pos)
{ 
    type = pos;
    float val = Value.toFloat();
    temp.clear();
    int inter = (int)val;
    float dec = val - (int)val;
    qInfo() << inter << dec << val<<"_10toN";
    if (pos != 10)
    {
        if (inter != 0)
        {
            while (inter != 0)
            {
                if (inter % pos < 10)
                {
                    temp.insert(0, (char)((inter % pos) + 48));
                }
                else
                {
                    temp.insert(0, (char)((inter % pos) + 55));
                }
                inter = inter / pos;
            }
        }
        else 
        {
            temp.append("0");
        }

        if (dec != 0.)
        {
            temp.append(".");
            for (int i = 0; i < 5; i++)
            {
                dec = dec * (float)pos;
                if (dec < 10.)
                {
                    Dec = (int)(dec + 48);
                    temp.append(Dec);
                }
                else
                {
                    Dec = (int)(dec  + 55);
                    temp.append(Dec);
                }
                dec = dec - (float)((int)dec);
            }
        }
        Value = temp;
    }
    qInfo() << type << "_10toN";
}

void Calculator_BaseNum::_Nto10(int src)
{
    qInfo() << Value << src<<type << "_Nto10";
    if (src != 10) 
    {
        QStringList Val = Value.split(".");
        QString Inter = Val[0];
        float val = 0.;
        for (int i = Inter.length() - 1, j = 0; j < Inter.length(); i--, j++)
        {
            if (Inter[i] <= '9'&& Inter[i] >= '0')
            {
                val += (float)((int)Inter[i].toLatin1() - 48) * pow(src, j);
            }
            else if(Inter[i] <= 'F' && Inter[i] >= 'A')
            {
                val += (float)((int)Inter[i].toLatin1() - 55) * pow(src, j);
            }

        }
        if (Val.length() > 1)
        {
            QString Dec = Val[1];
            for (int i = 0; i < Dec.length(); i++)
            {
                if (Dec[i] <= '9' && Dec[i] >= '0')
                {
                    val += (float)((int)Dec[i].toLatin1() - 48) * pow(src, -1 - i);
                }
                else if(Dec[i] <= 'F' && Dec[i] >= 'A')
                {
                    val += (float)((int)Dec[i].toLatin1() - 55) * pow(src, -1 - i);
                }

            }
        }
        Value = QString::number(val, 'f');
        type = 10;
        
    }
}

void Calculator_BaseNum::_2toN(int pos)
{
if (pos != 2) 
{
    _Nto10(2);
    _10toN(pos);
}
}

void Calculator_BaseNum::_8toN(int pos)
{
    if (pos != 8) 
    {
        _Nto10(8);
        _10toN(pos);
    }  
}

void Calculator_BaseNum::_16toN(int pos)
{
    qInfo() << Value << type << "16ToN";
    if (pos != 16)
    {       
        _Nto10(16);
        _10toN(pos);
    }
   
}

void Calculator_BaseNum::setWidgets(CalDay* w3)
{
    Caldaily = w3;
}

void Calculator_BaseNum::conversion(int pos)
{  
    qInfo() << "Type" << type;
    if (type == 10)
        _10toN(pos);
    else if (type == 2)
        _2toN(pos);
    else if (type == 8)
        _8toN(pos);
    else if (type == 16)
        _16toN(pos);

    if(pos == 2)ui->Label->setText("Bin:");
    if (pos == 8)ui->Label->setText("Oct:");
    if (pos == 10)ui->Label->setText("Dec:");
    if (pos == 16)ui->Label->setText("Hex:");

    ui->lineEdit_src->setText(Value);
 }

//按钮消息处理
void  Calculator_BaseNum::onButtonGroupClicked(QAbstractButton* btn)
{
    QString name = btn->text();
    Value = ui->lineEdit_src->text();
    if (name >= "0" && name <= "9"&& type == 10 || name == ".")
    {
        if (ui->lineEdit_src->text().remove('0')=="" && name != ".")
        {
            ui->lineEdit_src->clear();
        }

        if (ui->lineEdit_src->text().contains(".")) 
        {
            if(name != ".")ui->lineEdit_src->insert(name);
        }
        else 
        {
            ui->lineEdit_src->insert(name);
        }
        
    }
    else if (name >= "0" && name <= "1" && type == 2 || name == ".")
    {
        qInfo() << "type:" << type;
        if (ui->lineEdit_src->text().remove('0') == ""  && name != ".")
        {
            ui->lineEdit_src->clear();
        }

        if (ui->lineEdit_src->text().contains("."))
        {
            if (name != ".")ui->lineEdit_src->insert(name);
        }
        else
        {
            ui->lineEdit_src->insert(name);
        }
    }
    else if (name >= "0" && name <= "7" && type == 8 || name == ".")
    {
        if (ui->lineEdit_src->text().remove('0') == ""  && name != ".")
        {
            ui->lineEdit_src->clear();
        }
        
        if (ui->lineEdit_src->text().contains("."))
        {
            if (name != ".")ui->lineEdit_src->insert(name);
        }
        else
        {
            ui->lineEdit_src->insert(name);
        }
    }
    else if (name.length() == 1 && type == 16 || name == ".")
    {
        if (ui->lineEdit_src->text().remove('0') == ""  && name != ".")
        {
            ui->lineEdit_src->clear();
        }
        
        if (ui->lineEdit_src->text().contains("."))
        {
            if (name != ".")ui->lineEdit_src->insert(name);
        }
        else
        {
            ui->lineEdit_src->insert(name);
        }
    }
    else if (name == "Reset") 
    {
        Value = "0";
        ui->lineEdit_src->setText("0");
    }
    else if (name == "Delete") 
    {
        if (Value != "0") 
        {
            ui->lineEdit_src->setCursorPosition(ui->lineEdit_src->cursorPosition() - 1);
            ui->lineEdit_src->del();
        }
    }
    else if (name == "Bin")
    {
        conversion(2);
    }
    else if (name == "Dec")
    {
        qInfo() <<"Name:" << name;
        conversion(10);       
    }
    else if (name == "Oct")
    {
        conversion(8);
    }
    else if (name == "Hex")
    {
        conversion(16);
    }
    else if (name == "SwitchMode") 
    {
        currentPos = this->pos();
        this->hide();
        Caldaily->move(currentPos);
        Caldaily->show();
    }
}
