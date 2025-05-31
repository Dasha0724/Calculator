#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdexcept>
#include "support.h"
using namespace std;
bool calculated = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->num1, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num2, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num3, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num4, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num5, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num6, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num7, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num8, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num9, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->num0, &QPushButton::pressed, this, &MainWindow::digit_pressed);
    connect(ui->opLparen, &QPushButton::pressed, this, &MainWindow::add_brackets);
    connect(ui->opRparen, &QPushButton::pressed, this, &MainWindow::add_brackets);
    connect(ui->opPlus, &QPushButton::pressed, this, &MainWindow::binary_operation);
    connect(ui->opMinus, &QPushButton::pressed, this, &MainWindow::binary_operation);
    connect(ui->opDivide, &QPushButton::pressed, this, &MainWindow::binary_operation);
    connect(ui->opMultiply, &QPushButton::pressed, this, &MainWindow::binary_operation);
    connect(ui->opExp, &QPushButton::pressed, this, &MainWindow::binary_operation);
    connect(ui->opSqrt, &QPushButton::pressed, this, &MainWindow::unary_operation);
    connect(ui->dot, &QPushButton::pressed, this, &MainWindow::dot_pressed);
    connect(ui->plusminus, &QPushButton::clicked, this, &MainWindow::change_sign);
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
    calculated = false;
}


void MainWindow::on_opClearOne_pressed() {
    QString currentText = ui->label->text();

    if (currentText.isEmpty() || currentText == "0") {
        return;
    }
    currentText.chop(1);
    if (currentText.isEmpty() || currentText == "-") {
        currentText = "0";
    }
    else {
        currentText = currentText.trimmed();
    }

    ui->label->setText(currentText);
}


void MainWindow::on_opEqual_pressed() {
    QString input = ui->label->text();
    if (input.contains("Ошибка")) {
        ui->label->setText("0");
        ui->label_2->clear();
        calculated = false;
        return;
    }

    input.replace("√", "r");
    input = input.simplified();
    input.replace(" - ", "-");
    input.replace("- ", "-");
    input.replace(" -", "-");
    try {
        double result = evaluate(input.toStdString());
        ui->label_2->setText(ui->label->text() + " =");
        ui->label->setText(QString::number(result, 'g', 8));
        calculated = true;
    } catch (const runtime_error& e) {
        ui->label->setText("Ошибка");
        ui->label_2->clear();
        calculated = false;
    }
}


void MainWindow::change_sign() {
    QString currentText = ui->label->text().trimmed();
    if (currentText == "Ошибка") {
        ui->label->setText("-");
        return;
    }
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
                if (prevToken == "/" || prevToken == "*" || prevToken == "^"|| 
                    prevToken == "+" || prevToken == "-") {
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
    QString newOp = button->text();
    if (calculated) {
        calculated = false;
        ui->label_2->clear();
    }
    if (currentText == "0" && newOp != "-") {
        ui->label->setText("0 " + newOp);
        return;
    }
    QStringList tokens = currentText.split(' ', Qt::SkipEmptyParts);
    if (tokens.isEmpty()) {
        ui->label->setText("0 " + newOp);
        return;
    }
    QString lastToken = tokens.last();
    if (lastToken == "+" || lastToken == "-" || lastToken == "*" ||
        lastToken == "/" || lastToken == "^") {
        tokens[tokens.size()-1] = newOp;
        ui->label->setText(tokens.join(" "));
    }
    else {
        ui->label->setText(currentText + " " + newOp);
    }
}


void MainWindow::add_brackets() {
    QPushButton* button = (QPushButton*)sender();
    QString currentText = ui->label->text();
    QString bracket = button->text();

    if (calculated) {
        ui->label->setText("0");
        ui->label_2->clear();
        calculated = false;
    }
    if (currentText == "0") {
        ui->label->setText(bracket);
    } else {
        ui->label->setText(currentText + " " + bracket);
    }
}


void MainWindow::unary_operation() {
    QPushButton* button = (QPushButton*)sender();
    QString currentText = ui->label->text();

    if (calculated) {
        ui->label->setText("0");
        ui->label_2->clear();
        calculated = false;
    }

    if (button == ui->opSqrt) {
        QStringList tokens = currentText.split(' ', Qt::SkipEmptyParts);
        if (!tokens.isEmpty() && tokens.last() == "√") {
            return;
        }

        if (currentText == "0") {
            ui->label->setText("√");
        } else {
            ui->label->setText(currentText + " √");
        }
    }
}


void MainWindow::dot_pressed()
{
    if (calculated) {
        ui->label->setText("0");
        ui->label_2->clear();
        calculated = false;
    }
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


void MainWindow::digit_pressed() {
    QPushButton *button = (QPushButton*)sender();
    QString currentText = ui->label->text();

    if (currentText == "Ошибка") {
        ui->label->setText(button->text());
        return;
    }

    if (calculated) {
        currentText = "0";
        ui->label_2->clear();
        calculated = false;
    }

    if (currentText == "0") {
        ui->label->setText(button->text());
    } else {
        QStringList parts = currentText.split(' ', Qt::SkipEmptyParts);
        if (!parts.isEmpty()) {
            QString lastPart = parts.last();

            if (lastPart == "+" || lastPart == "-" || lastPart == "*" ||
                lastPart == "/" || lastPart == "^" || lastPart == "√") {
                ui->label->setText(currentText + " " + button->text());
                return;
            }

            if (lastPart == "(") {
                ui->label->setText(currentText + button->text());
                return;
            }
        }
        ui->label->setText(currentText + button->text());
    }
}
