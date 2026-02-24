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
private slots:
    void on_addButton_clicked();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_kurseur_editingFinished();

    void on_wyczyscbutton_clicked();

private:
    Ui::MainWindow *ui;
    double kursEur;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
};
#endif // MAINWINDOW_H
