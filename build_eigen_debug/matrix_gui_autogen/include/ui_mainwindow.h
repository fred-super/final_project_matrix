/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QSpinBox *spinBoxSize;
    QHBoxLayout *horizontalLayoutTables;
    QVBoxLayout *vboxLayout;
    QLabel *label;
    QTableWidget *tableWidget;
    QVBoxLayout *vboxLayout1;
    QLabel *label1;
    QTableWidget *tableVectorB;
    QHBoxLayout *horizontalLayoutBtns;
    QPushButton *btnImport;
    QPushButton *btnExport;
    QPushButton *btnSolve;
    QPushButton *btnCalculate;
    QLabel *labelResult;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(700, 500);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        spinBoxSize = new QSpinBox(centralwidget);
        spinBoxSize->setObjectName("spinBoxSize");
        spinBoxSize->setMinimum(1);
        spinBoxSize->setValue(3);

        verticalLayout->addWidget(spinBoxSize);

        horizontalLayoutTables = new QHBoxLayout();
        horizontalLayoutTables->setObjectName("horizontalLayoutTables");
        vboxLayout = new QVBoxLayout();
        vboxLayout->setObjectName("vboxLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        vboxLayout->addWidget(label);

        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName("tableWidget");

        vboxLayout->addWidget(tableWidget);


        horizontalLayoutTables->addLayout(vboxLayout);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setObjectName("vboxLayout1");
        label1 = new QLabel(centralwidget);
        label1->setObjectName("label1");

        vboxLayout1->addWidget(label1);

        tableVectorB = new QTableWidget(centralwidget);
        tableVectorB->setObjectName("tableVectorB");
        tableVectorB->setMaximumWidth(100);

        vboxLayout1->addWidget(tableVectorB);


        horizontalLayoutTables->addLayout(vboxLayout1);


        verticalLayout->addLayout(horizontalLayoutTables);

        horizontalLayoutBtns = new QHBoxLayout();
        horizontalLayoutBtns->setObjectName("horizontalLayoutBtns");
        btnImport = new QPushButton(centralwidget);
        btnImport->setObjectName("btnImport");

        horizontalLayoutBtns->addWidget(btnImport);

        btnExport = new QPushButton(centralwidget);
        btnExport->setObjectName("btnExport");

        horizontalLayoutBtns->addWidget(btnExport);

        btnSolve = new QPushButton(centralwidget);
        btnSolve->setObjectName("btnSolve");

        horizontalLayoutBtns->addWidget(btnSolve);


        verticalLayout->addLayout(horizontalLayoutBtns);

        btnCalculate = new QPushButton(centralwidget);
        btnCalculate->setObjectName("btnCalculate");

        verticalLayout->addWidget(btnCalculate);

        labelResult = new QLabel(centralwidget);
        labelResult->setObjectName("labelResult");
        QFont font;
        font.setPointSize(11);
        labelResult->setFont(font);
        labelResult->setWordWrap(true);

        verticalLayout->addWidget(labelResult);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Matrix Processor MIPT Edition", nullptr));
        spinBoxSize->setPrefix(QCoreApplication::translate("MainWindow", "\320\240\320\260\320\267\320\274\320\265\321\200 (N): ", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\321\202\321\200\320\270\321\206\320\260 A", nullptr));
        label1->setText(QCoreApplication::translate("MainWindow", "\320\222\320\265\320\272\321\202\320\276\321\200 B", nullptr));
        btnImport->setText(QCoreApplication::translate("MainWindow", "\360\237\223\202 \320\227\320\260\320\263\321\200\321\203\320\267\320\270\321\202\321\214", nullptr));
        btnExport->setText(QCoreApplication::translate("MainWindow", "\360\237\222\276 \320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        btnSolve->setText(QCoreApplication::translate("MainWindow", "\342\232\241 \320\240\320\265\321\210\320\270\321\202\321\214 \320\241\320\233\320\220\320\243", nullptr));
        btnCalculate->setText(QCoreApplication::translate("MainWindow", "\320\222\321\213\321\207\320\270\321\201\320\273\320\270\321\202\321\214 \320\276\320\277\321\200\320\265\320\264\320\265\320\273\320\270\321\202\320\265\320\273\321\214 det(A)", nullptr));
        labelResult->setText(QCoreApplication::translate("MainWindow", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202 \320\277\320\276\321\217\320\262\320\270\321\202\321\201\321\217 \320\267\320\264\320\265\321\201\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
