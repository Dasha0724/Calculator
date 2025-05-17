#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "support.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->num1,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num2,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num3,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num4,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num5,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num6,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num7,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num8,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num9,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->num0,SIGNAL(pressed()),this,SLOT(digit_pressed()));
    connect(ui->opLparen,SIGNAL(pressed()),this,SLOT(add_brackets()));
    connect(ui->opRparen,SIGNAL(pressed()),this,SLOT(add_brackets()));
    connect(ui->opPlus,SIGNAL(pressed()),this,SLOT(binary_operation()));
    connect(ui->opMinus,SIGNAL(pressed()),this,SLOT(binary_operation()));
    connect(ui->opDivide,SIGNAL(pressed()),this,SLOT(binary_operation()));
    connect(ui->opMultiply,SIGNAL(pressed()),this,SLOT(binary_operation()));
    connect(ui->opExp,SIGNAL(pressed()),this,SLOT(binary_operation()));
    connect(ui->opSqrt, SIGNAL(pressed()), this, SLOT(unary_operation()));
    connect(ui->dot, SIGNAL(pressed()), this, SLOT(dot_pressed()));
    connect(ui->plusminus,SIGNAL(clicked()),this,SLOT(change_sign()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Plus_Minus() {
    QString input = ui->label->text();
    QStringList parts = input.split(' ', Qt::SkipEmptyParts);

    if (parts.isEmpty())
        return;
    int lastNumberIndex = -1;
    for (int i = parts.size()-1; i >= 0; i--) {
        bool isNumber;
        parts[i].toDouble(&isNumber);
        if (isNumber || (parts[i].startsWith('-') && parts[i].mid(1).toDouble(&isNumber))) {
            lastNumberIndex = i;
            break;
        }
    }
    if (lastNumberIndex == -1) return;

    QString lastNumber = parts[lastNumberIndex];
    if (lastNumber.startsWith('-')) {
        parts[lastNumberIndex] = lastNumber.mid(1);
    } else {
       if (lastNumberIndex > 0 &&
            (parts[lastNumberIndex-1] == "/" ||
             parts[lastNumberIndex-1] == "*" ||
             parts[lastNumberIndex-1] == "^")) {
            parts[lastNumberIndex] = "(-" + lastNumber + ")";
        } else {
            parts[lastNumberIndex] = "-" + lastNumber;
        }
    }
    ui->label->setText(parts.join(" "));
    ui->label_2->setText(parts.join(" "));
}


void MainWindow::on_opClear_pressed()
{
    ui->label->setText("0");
    ui->label_2->clear();
}


void MainWindow::on_opEqual_pressed() {
    QString input = ui->label->text();
    input.replace("√", "r");
    input = input.simplified();

    try {
        double result = evaluate(input.toStdString());
        ui->label_2->setText(ui->label->text());
        ui->label->setText(QString::number(result, 'g', 15));
    }
    catch (const runtime_error& e) {
        ui->label->setText(QString::fromStdString(e.what()));
    }
}


void MainWindow::change_sign() {
    QString currentText = ui->label->text().trimmed();
    if (currentText.isEmpty() || currentText == "0") {
        ui->label->setText("-");
        return;
    }
    QStringList tokens = currentText.split(' ', Qt::SkipEmptyParts);
    if (tokens.isEmpty())
        return;
    QString lastToken = tokens.last();
    bool isNumber;
    double number;
    if (lastToken.startsWith("(-") && lastToken.endsWith(")")) {
        QString numStr = lastToken.mid(2, lastToken.length()-3);
        number = numStr.toDouble(&isNumber);
        if (isNumber) {
            tokens[tokens.size()-1] = numStr;
            ui->label->setText(tokens.join(" "));
            return; }
    }
    number = lastToken.toDouble(&isNumber);
    if (isNumber) {
        if (lastToken.startsWith('-')) {
            tokens[tokens.size()-1] = lastToken.mid(1);
        } else {
            if (tokens.size() > 1) {
                QString prevToken = tokens[tokens.size()-2];
                if (prevToken == "/" || prevToken == "*" || prevToken == "^") {
                    tokens[tokens.size()-1] = "(-" + lastToken + ")";
                } else {
                    tokens[tokens.size()-1] = "-" + lastToken;
                }
            } else {
                tokens[tokens.size()-1] = "-" + lastToken;
            }
        }
        ui->label->setText(tokens.join(" "));
    }
}


void MainWindow::binary_operation() {
    QPushButton* button = (QPushButton*)sender();
    QString currentText = ui->label->text();
    if(currentText == "0") {
        ui->label->setText("0 " + button->text());
    } else {
        ui->label->setText(currentText + " " + button->text());
    }
    qDebug() << "Текущий ввод: " << ui->label->text();
}


void MainWindow::add_brackets() {
    QPushButton* button = (QPushButton*)sender();
    QString currentText = ui->label->text();
    if (currentText == "0") {
        ui->label->setText(button->text());
    } else {
        ui->label->setText(currentText + " " + button->text());
    }
}


void MainWindow::unary_operation() {
    QPushButton* button = (QPushButton*)sender();
    QString currentText = ui->label->text();

    if (button == ui->opSqrt) {
        if(currentText == "0") {
            ui->label->setText("√");
        } else {
            ui->label->setText(currentText + " √"); }
    }
}


void MainWindow::dot_pressed()
{
    QString currentText = ui->label->text();
    if(currentText.isEmpty() || currentText == "0") {
        ui->label->setText("0.");
        return;
    }
    QStringList parts = currentText.split(' ', Qt::SkipEmptyParts);
    if(!parts.isEmpty()) {
        QString lastPart = parts.last();
        if(lastPart == "√") {
            ui->label->setText(currentText + " 0.");
            return;
        }
        bool isNumber;
        lastPart.toDouble(&isNumber);
        if(isNumber) {
            if(!lastPart.contains('.')) {
                ui->label->setText(currentText + ".");
            }
            return;
        }
        if(lastPart == "+" || lastPart == "-" || lastPart == "*" ||
            lastPart == "/" || lastPart == "^" || lastPart == "(") {
            ui->label->setText(currentText + " 0.");
            return;
        }
    }
}


void MainWindow::digit_pressed(){
    QPushButton *button = (QPushButton*)sender();
    QString currentText = ui->label->text();
    if(button == ui->opSqrt) {
        if(currentText == "0") {
            ui->label->setText("√");
        } else {
            ui->label->setText(currentText + " √");
        }
        return;
    }
    if(currentText == "0") {
        ui->label->setText(button->text());
        return;
    }
    QStringList parts = currentText.split(' ', Qt::SkipEmptyParts);
    if(!parts.isEmpty()) {
        QString lastPart = parts.last();
        if(lastPart == "+" || lastPart == "-" || lastPart == "*" ||
            lastPart == "/" || lastPart == "^" || lastPart == "(" ||
            lastPart == "√") {
            ui->label->setText(currentText + " " + button->text());
            return;
        }
    }
    ui->label->setText(currentText + button->text());
}
