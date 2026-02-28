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
