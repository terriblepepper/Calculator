#include "Calculator.h"
#include "BaseNum.h"

using namespace std;
QPoint currentPos;//记录当前窗口位置
//字符串转数字
template <class Type> Type stringToNum(const string str) { istringstream iss(str); Type num; iss >> num; return num; }

//表达式计算
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3; // 指数运算符的优先级更高
    return 0;
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isValid(const std::string& expression) {
    std::stack<char> stack;
    char t = '0';
    bool flag = true;
    for (char c : expression) {
        if ((t == '+' || t == '-' || t == '*' || t == '/' || t == '^') && !(c >= '0' && c <= '9' || c == '(')) 
        {
            qInfo() << t << c;
            flag = false;
            break;
        }
        if (c == '(') {
            stack.push(c);
        }
        else if (c == ')') {
            if (stack.empty() || stack.top() != '(') {
                return false;
            }
            stack.pop();
        }
        t = c;
    }

    if (expression.back() == '+' || expression.back() == '-' || expression.back() == '*' || expression.back() == '/' || expression.back() == '^')
        flag = false;

    return stack.empty()&&flag;
}

double applyOp(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return b != 0 ? a / b : throw std::runtime_error("Division by zero");
    case '^': return pow(a, b);
    default: throw std::runtime_error("Unsupported operator");
    }
}

double evaluate(const std::string& expression) {
    if (!isValid(expression)) {
        throw std::runtime_error("Unbalanced parentheses");
    }

    std::stack<double> values;
    std::stack<char> ops;

    bool expectingValue = true;

    for (int i = 0; i < expression.length(); ++i) {
        if (expression[i] == ' ') {
            continue;
        }
        else if (expression[i] == '(') {
            ops.push(expression[i]);
            expectingValue = true;
        }
        else if (isdigit(expression[i]) || expression[i] == '.') {
            std::string numStr;
            numStr += expression[i];
            i++;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i];
                i++;
            }
            i--;

            double value = std::stod(numStr);
            if (!expectingValue) {
                throw std::runtime_error("Invalid expression: Two consecutive values without operator");
            }
            values.push(value);
            expectingValue = false;
        }
        else if (expression[i] == 'e' || expression[i] == 'E') {
            int exponent = 0;
            i++;
            int sign = 1;
            if (expression[i] == '+') {
                i++;
            }
            else if (expression[i] == '-') {
                sign = -1;
                i++;
            }
            while (i < expression.length() && isdigit(expression[i])) {
                exponent = exponent * 10 + (expression[i] - '0');
                i++;
            }
            i--;

            double& top = values.top();
            top *= pow(10, sign * exponent);
        }
        else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top();
                values.pop();

                double val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            ops.pop();
            expectingValue = false;
        }
        else if (isOperator(expression[i])) {
            if (expectingValue && (expression[i] == '-' || expression[i] == '+')) {
                std::string numStr;
                numStr += expression[i];
                i++;
                while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                    numStr += expression[i];
                    i++;
                }
                i--;

                double value = std::stod(numStr);
                values.push(value);
            }
            else {
                while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                    double val2 = values.top();
                    values.pop();

                    double val1 = values.top();
                    values.pop();

                    char op = ops.top();
                    ops.pop();

                    values.push(applyOp(val1, val2, op));
                }
                ops.push(expression[i]);
                expectingValue = true;
            }
        }
        else {
            throw std::runtime_error("Invalid character");
        }
    }

    while (!ops.empty()) {
        double val2 = values.top();
        values.pop();

        double val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(val1, val2, op));
    }
    return values.top();
}


// 构造函数
Calculator::Calculator(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::CalculatorClass())
{
    ui->setupUi(this); // 设置UI
    this->setWindowTitle("Calculator");
    this->setWindowIcon(QIcon(":/iamges/bk.png"));
    ini_Ui(); // 初始化UI
}

// 析构函数
Calculator::~Calculator()
{
    delete ui; // 删除UI
}

// 初始化UI函数
void Calculator::ini_Ui()
{
    auto buttonGroup = new QButtonGroup(this); // 创建按钮组
    auto btnList = findChildren<QPushButton*>(); // 查找所有QPushButton子控件
    for (auto btn : btnList)
    {
        buttonGroup->addButton(btn); // 将按钮添加到按钮组
    }
    connect(buttonGroup, QOverload<QAbstractButton*>::of(&QButtonGroup::buttonClicked), this, &Calculator::onButtonGroupClicked); // 连接按钮组的点击信号与槽函数
   
}

// 判断按钮是否为运算符
bool Calculator::btnIsSign(QString btn)
{
    if (btn == "+" || btn == "-" || btn == QString(QChar(0x00D7)) || btn == QString(QChar(0x00F7)))
        return true;
    return false;
}

//乘除号替换函数
void Calculator::Replace(QString* Txt) {
    Txt->replace(QString(QChar(0x00D7)), "*");
    Txt->replace(QString(QChar(0x00F7)), "/");
}

//计算表达式函数
void Calculator::Eva(Ui::CalculatorClass* ui)
{
    
    if (ui->lineEdit->text().contains("+") || ui->lineEdit->text().contains("-") || ui->lineEdit->text().contains(QString(QChar(0x00D7))) || ui->lineEdit->text().contains(QString(QChar(0x00F7))) || ui->lineEdit->text().contains("(") || ui->lineEdit->text().contains(")"))
    {
        try {
            CTxt = ui->lineEdit->text();
            Calculator::Replace(&CTxt);
            result = evaluate(CTxt.toStdString());
            Txt = QString::number(result);
            ExTxt = ui->lineEdit->text() + "=" + Txt;
            ui->lineEdit->setText(Txt);
            ui->explineEdit->setText(ExTxt);
        }
        catch (const exception& e) {
            ui->lineEdit->setText("Error");
            ui->explineEdit->clear();
        }
    }
    else if (!ui->lineEdit->text().contains("+") && !ui->lineEdit->text().contains("-") && !ui->lineEdit->text().contains(QString(QChar(0x00D7))) && !ui->lineEdit->text().contains(QString(QChar(0x00F7))))
    {
        ExTxt = ui->lineEdit->text();
        Txt = ui->lineEdit->text();
        result = stringToNum<double>(ui->lineEdit->text().toStdString());
        ui->explineEdit->setText(ExTxt);
    }
    else
    {
        ui->lineEdit->setText("Error");
        ui->explineEdit->clear();
    }
   
}

void Calculator::setWidgets(Calculator_BaseNum* w2)
{
    CalBase = w2;
}

// 按钮组点击事件处理函数
void Calculator::onButtonGroupClicked(QAbstractButton* btn)
{
    QString name = btn->text();
    double temp1 = stringToNum<double>(ui->lineEdit->text().toStdString());
    int temp2 = stringToNum<int>(ui->lineEdit->text().toStdString());
    if (name >= "0" && name <= "9" || name == "."||name == "+" || name == "-" || name == QString(QChar(0x00D7)) || name == QString(QChar(0x00F7)) || name == "(" || name == ")" || name == "x^y") // 如果按钮是数字或小数点以及四则运算
    {

         if (ui->lineEdit->text() == "0" && !(name == "." || name == "+" || name == "-" || name == QString(QChar(0x00D7)) || name == QString(QChar(0x00F7)) || name == "x^y"))
         {
                ui->lineEdit->clear(); // 如果当前值是0且按钮不是小数点及括号和其他算术符号，则清空行编辑框
         }

         if (ui->lineEdit->text() == "Error")
         {
                ui->lineEdit->clear(); // 如果Error则清空编辑框
         }

         if (name >= "0" && name <= "9" || name == "." || ui->lineEdit->text() != "0")
         {
             if (Calculator::btnIsSign(preBtn) && Calculator::btnIsSign(name))
             {
                    ui->lineEdit->setCursorPosition(ui->lineEdit->cursorPosition() - 1); // 设置光标位置
                    ui->lineEdit->del(); // 删除光标位置的字符
             }

             if (name != "x^y")ui->lineEdit->insert(name); // 在行编辑框中插入按钮的文本
             else ui->lineEdit->insert("^(");
         }

        
    }
    else if (name == "C") // 清除所有输入
    {
        ui->lineEdit->clear(); // 清空行编辑框
        ui->explineEdit->clear(); // 清空表达式编辑框
      
        ui->lineEdit->insert("0");//恢复初值0
    }
    else if (name == "CE") // 清除当前输入
    {
        ui->lineEdit->clear(); // 清空行编辑框
        if (ExTxt.contains(ui->lineEdit->text()))
        ui->lineEdit->insert("0");//恢复初值0
    }
    else if (name == "DEL") // 删除最后一个字符
    {
        if (ui->lineEdit->text().length()>1) 
        {
            ui->lineEdit->setCursorPosition(ui->lineEdit->cursorPosition() - 1); // 设置光标位置
            ui->lineEdit->del(); // 删除光标位置的字符
        }
        else
        {
            ui->lineEdit->setText("0");
        }
    }
    else if (name == "CountDown" && temp1 != 0)
    {
        try
        {
            Calculator::Eva(ui);
            ui->explineEdit->clear();
            if (!ui->lineEdit->text().contains("Error"))
            {
                ui->explineEdit->setText("1/" + QString::number(result) + "=");
                result = 1.0/result;
                ui->lineEdit->setText(QString::number(result));
            }
        }
        catch (const exception& e)
        {
            ui->lineEdit->setText("Error");
        }
    }
    else if (name == "x^2")
    {
        try
        {
            Calculator::Eva(ui);
            ui->explineEdit->clear();
            if (!ui->lineEdit->text().contains("Error"))
            {
                ui->explineEdit->setText(QString::number(result) + "^2=");
                result *= result;
                ui->lineEdit->setText(QString::number(result));
            }
        }
        catch (const exception& e)
        {
            ui->lineEdit->setText("Error");
        }
    }
    else if (name == "%")
    {
        try
        {
            Calculator::Eva(ui);
            ui->explineEdit->clear();
            if (!ui->lineEdit->text().contains("Error"))
            {
                ui->explineEdit->setText(QString::number(result) + "/10=");
                result /= 10.0;
                ui->lineEdit->setText(QString::number(result));
            }
        }
        catch (const exception& e)
        {
            ui->lineEdit->setText("Error");
        }
    }
    else if (name == "_/x" && temp1 >= 0)
    {
        try
        {
            Calculator::Eva(ui);
            ui->explineEdit->clear();
            if (!ui->lineEdit->text().contains("Error"))
            {
                ui->explineEdit->setText("_/(" + QString::number(result) + ")=");
                result = sqrt(result);
                ui->lineEdit->setText(QString::number(result));
            }
        }
        catch (const exception& e)
        {
            ui->lineEdit->setText("Error");
        }
    }
    else if (name == "sin")
    {
    try
    {
        Calculator::Eva(ui);
        ui->explineEdit->clear();
        if (!ui->lineEdit->text().contains("Error"))
        {
            ui->explineEdit->setText("sin(" + QString::number(result) + ")=");
            result = sin(stringToNum<double>(ui->lineEdit->text().toStdString()) / 180.0 * acos(-1));
            ui->lineEdit->setText(QString::number(result));
        }
    }
    catch (const exception& e)
    {
        ui->lineEdit->setText("Error");
    }
    }
    else if (name == "cos")
    {
        try
        {
        Calculator::Eva(ui);
        ui->explineEdit->clear();
        if (!ui->lineEdit->text().contains("Error"))
        {
            ui->explineEdit->setText("cos(" + QString::number(result) + ")=");
            result = cos(stringToNum<double>(ui->lineEdit->text().toStdString()) / 180.0 * acos(-1));
            ui->lineEdit->setText(QString::number(result));
        }
        }
        catch (const exception& e)
        {
            ui->lineEdit->setText("Error");
        }
    }
    else if (name == "tan" && !(temp1-temp2 == 0 && (temp2%180==90||temp2%180==-90)))
    {
        qInfo() << result;
        try 
        {
            Calculator::Eva(ui);
            ui->explineEdit->clear();
            if (!ui->lineEdit->text().contains("Error"))
            {
                ui->explineEdit->setText("tan(" + QString::number(result) + ")=");
                result = tan(stringToNum<double>(ui->lineEdit->text().toStdString()) / 180.0 * acos(-1));
                ui->lineEdit->setText(QString::number(result));                
            }
        }
        catch (const exception& e)
        {
            ui->lineEdit->setText("Error");
        }     
    }
    else if (name == "lg" && temp1 > 0)
    {
        try
        {
            Calculator::Eva(ui);
            ui->explineEdit->clear();
            if (!ui->lineEdit->text().contains("Error"))
            {
                ui->explineEdit->setText("lg(" + QString::number(result) + ")=");
                result = log10(stringToNum<double>(ui->lineEdit->text().toStdString()));
                ui->lineEdit->setText(QString::number(result));
            }
        }
        catch (const exception& e)
        {
            ui->lineEdit->setText("Error");
        }
    }
    else if (name == "ln" && temp1 > 0)
    {
       
        try 
        {
            Calculator::Eva(ui);
            ui->explineEdit->clear();
            if (!ui->lineEdit->text().contains("Error"))
            {
                ui->explineEdit->setText("ln(" + QString::number(result) + ")=");
                result = log(stringToNum<double>(ui->lineEdit->text().toStdString()));
                ui->lineEdit->setText(QString::number(result));
            }
        }
        catch (const exception& e) 
        {
            ui->lineEdit->setText("Error");
        }
    }
    else if (name == "=") // 等于运算
    {
        if (ui->lineEdit->text() != "0") 
        {
            Calculator::Eva(ui);
        }    
    }
    else if (name == "SwitchMode")
    {
    currentPos = this->pos();
    this->hide();
    CalBase->move(currentPos);
    CalBase->show();
    }  
    preBtn = name; // 保存当前按钮文本
}
