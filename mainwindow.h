/*
    SureBet Calculator
    Copyright (C) 2026 tutekxd123

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QHBoxLayout>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum Cols_Nums{
    COL_ODD = 0,
    COL_ODD_EFFECTIVE = 1,
    COL_ODD_STAKE= 2,
    COL_COMMISION =3,
    COL_TYPE_COMMISSION = 4,
    COL_ODD_LAY = 5,
    COL_LIBALITY_LAY = 6,
    COL_CURRENCY = 7,
    COL_LAY = 8,
    COL_REVENUE = 9
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void addRow();
    void calcResult();
    void updateKurs(QNetworkReply *reply);
    void onTableItemChanged(QTableWidgetItem* item);
    void CalculateOdds(int row, int col);
    void ResetTable();
    void TryParse(int row, int col);
    double RoundTo(double number, double roundto);
private slots:
    void on_addButton_clicked();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_kurseur_editingFinished();

    void on_wyczyscbutton_clicked();

    void on_BudzetInput_editingFinished();


    void on_roundto_editingFinished();

private:
    Ui::MainWindow *ui;
    double kursEur;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};
#endif // MAINWINDOW_H
