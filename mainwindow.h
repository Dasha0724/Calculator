#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void digit_pressed();
    void on_opClear_pressed();
    void on_opEqual_pressed();
    void binary_operation();
    void add_brackets();
    void unary_operation();
    void Plus_Minus();
    void change_sign();
    void dot_pressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
