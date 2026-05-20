#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../include/matrix.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_spinBoxSize_valueChanged(int arg1);
    void on_btnCalculate_clicked();
    void on_btnImport_clicked(); 
    void on_btnExport_clicked(); 
    void on_btnSolve_clicked();

private:
    Ui::MainWindow *ui;
};

#endif 