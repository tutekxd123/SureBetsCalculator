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
#include <QtWidgets/QPlainTextEdit>
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
    QPlainTextEdit *BudzetInput;
    QPushButton *pushButton_2;
    QLabel *label_2;
    QLineEdit *Sumaodwrotnosci;
    QLabel *label_3;
    QLineEdit *kurseur;
    QPushButton *wyczyscbutton;
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
        pushButton->setGeometry(QRect(90, 280, 80, 24));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 330, 91, 16));
        BudzetInput = new QPlainTextEdit(centralwidget);
        BudzetInput->setObjectName("BudzetInput");
        BudzetInput->setGeometry(QRect(90, 320, 111, 31));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(940, 270, 80, 24));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(210, 320, 101, 31));
        Sumaodwrotnosci = new QLineEdit(centralwidget);
        Sumaodwrotnosci->setObjectName("Sumaodwrotnosci");
        Sumaodwrotnosci->setGeometry(QRect(320, 320, 113, 31));
        Sumaodwrotnosci->setReadOnly(true);
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(440, 330, 101, 16));
        kurseur = new QLineEdit(centralwidget);
        kurseur->setObjectName("kurseur");
        kurseur->setGeometry(QRect(540, 320, 113, 31));
        wyczyscbutton = new QPushButton(centralwidget);
        wyczyscbutton->setObjectName("wyczyscbutton");
        wyczyscbutton->setGeometry(QRect(940, 320, 80, 24));
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
        addButton->setText(QCoreApplication::translate("MainWindow", "Dodaj Mecz", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Usu\305\204 Mecz", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Bud\305\274et zak\305\202adu:", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Oblicz", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Suma odwrotno\305\233ci:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "KURS EURO/PLN:", nullptr));
        wyczyscbutton->setText(QCoreApplication::translate("MainWindow", "Wyczy\305\233\304\207", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
