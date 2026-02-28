/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableWidget *tableWidget;
    QPushButton *addButton;
    QPushButton *pushButton;
    QLabel *label;
    QPushButton *pushButton_2;
    QLabel *label_3;
    QLineEdit *kurseur;
    QPushButton *wyczyscbutton;
    QLabel *label_4;
    QLineEdit *BudzetInput;
    QLineEdit *roundto;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1036, 600);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(0, 0, 1021, 261));
        sizePolicy.setHeightForWidth(tableWidget->sizePolicy().hasHeightForWidth());
        tableWidget->setSizePolicy(sizePolicy);
        tableWidget->setAutoFillBackground(false);
        tableWidget->setShowGrid(true);
        addButton = new QPushButton(centralwidget);
        addButton->setObjectName("addButton");
        addButton->setGeometry(QRect(0, 280, 80, 24));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(90, 280, 91, 24));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 330, 91, 16));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(940, 270, 80, 24));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(400, 330, 101, 16));
        kurseur = new QLineEdit(centralwidget);
        kurseur->setObjectName("kurseur");
        kurseur->setGeometry(QRect(500, 320, 113, 31));
        wyczyscbutton = new QPushButton(centralwidget);
        wyczyscbutton->setObjectName("wyczyscbutton");
        wyczyscbutton->setGeometry(QRect(940, 320, 80, 24));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(210, 330, 81, 16));
        BudzetInput = new QLineEdit(centralwidget);
        BudzetInput->setObjectName("BudzetInput");
        BudzetInput->setGeometry(QRect(90, 320, 113, 31));
        roundto = new QLineEdit(centralwidget);
        roundto->setObjectName("roundto");
        roundto->setGeometry(QRect(280, 320, 113, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1036, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Kalkulator SureBet", nullptr));
        addButton->setText(QCoreApplication::translate("MainWindow", "Add Match", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Remove Match", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Bud\305\274et zak\305\202adu:", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Calculate", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "KURS EURO/PLN:", nullptr));
        kurseur->setText(QCoreApplication::translate("MainWindow", "4.20", nullptr));
        wyczyscbutton->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Round To:", nullptr));
        roundto->setText(QCoreApplication::translate("MainWindow", "0.01", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
