#include "mainwindow.h"
#include "./ui_mainwindow.h"


void  MainWindow::CalculateOdds(int row, int col){
    double value = ui->tableWidget->item(row,col)->text().toDouble();
    if(value<=0) return;
    double newvalue = value/(value-1);
    int coltochange = col==0? 5:0;
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->item(row,coltochange)->setText(QString::number(newvalue));
    ui->tableWidget->blockSignals(false);
}
void MainWindow::onTableItemChanged(QTableWidgetItem* item){
    if(!item) return;

    int col = item->column();
    int row = item->row();

    if(col == 0 || col == 5){
        CalculateOdds(row,col);
    }
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //Teoretycznie powinnismy to robic w main.cpp ale i tak aplikacja jest jednostronnicowa(pobieranie kursu),najlepiej async
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,this, &MainWindow::updateKurs);

    manager->get(QNetworkRequest(QUrl("https://api.nbp.pl/api/exchangerates/rates/a/EUR/?format=JSON")));
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels({"Odd","Real Odd", "Stake","Commision(%)","Type","Kurs Lay","Liability Lay","Waluta","Lay?"});
    //Tylko jeden Row Select
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->tableWidget, &QTableWidget::itemChanged,this, &MainWindow::onTableItemChanged);
    MainWindow::addRow();


}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    MainWindow::addRow();
}
void MainWindow::updateKurs(QNetworkReply* reply)
{
    if(reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Błąd:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    qDebug() << "Odpowiedź:" << data;
    //Parsing Data And Update UI
    std::string stringparsing = data.toStdString();
    size_t findstring = stringparsing.find("mid");
    if(findstring==std::string::npos){
        return;
    }
    size_t findstring2 = stringparsing.find("}",findstring);
    if(findstring2==std::string::npos){
        return;
    //
    }
    std::string sliced = stringparsing.substr(findstring+5,findstring2-(findstring+5));
    qDebug()<<"Parsed: "<<sliced;

    ui->kurseur->setText(QString::fromStdString(sliced));
    this->kursEur = std::stod(sliced);
    reply->deleteLater();
}


void MainWindow::on_pushButton_clicked()
{
    auto rowList = ui->tableWidget->selectionModel()->selectedRows();
    if(rowList.count()>0){
        int rowNumber = rowList.constFirst().row();
        ui->tableWidget->removeRow(rowNumber);
    }
    else{
        int count = ui->tableWidget->rowCount();
        if(count>0){
            ui->tableWidget->removeRow(count-1);
        }
    }


}

void MainWindow::calcResult(){
    ui->tableWidget->blockSignals(true);
    double suminvert =0;
    double firstStake = -1;
    double FirstOdd = -1;
    bool okbudzet = false;
    double budzet = ui->BudzetInput->toPlainText().toDouble(&okbudzet);
    double converplntoeur = this->kursEur;
    int LayCount = 0;
    int layRow = -1;
    if(converplntoeur<1){
        qDebug()<<"Error EUR KURS";
        ui->tableWidget->blockSignals(false);
        return;
    }
    auto rows = ui->tableWidget->rowCount();
    for(int i=0;i<rows;i++){
        bool isLay = ((QCheckBox*)ui->tableWidget->cellWidget(i,8))->isChecked();
        if(isLay){
            LayCount++;
            layRow = i;
        }
        double marza = ui->tableWidget->item(i,3)->text().toDouble();
        double commission = ui->tableWidget->item(i,3)->text().toDouble() / 100.0;
        marza = (100-marza)/100; //marza jest odwrotonoscia!
        double kurs =  ui->tableWidget->item(i,0)->text().toDouble();
        double OddLay = ui->tableWidget->item(i,5)->text().toDouble();
        if(kurs<=0){
            qDebug()<<"Kurs niepowiodl";
            ui->tableWidget->blockSignals(false);
            return;
        }
        bool CalcOddLay = ((QCheckBox*)ui->tableWidget->cellWidget(i,8))->isChecked();
        if(OddLay>0 && CalcOddLay){
            qDebug()<<1/(OddLay-commission);
            suminvert += 1/(OddLay-commission);
        }
        else{

        }
        //Mozna juz tutaj step nr.1
        QComboBox* marzabox =  (QComboBox*)ui->tableWidget->cellWidget(i,4);
        std::string marzastring = marzabox->currentText().toStdString();
        //Read Marza

        double oddeffective = 0;
        if(marzastring=="Bet"){
            //Podatek taki jak w Polsce
            oddeffective = kurs*marza;
        }
        else{
            //Podatek od Win
            oddeffective = 1+(kurs-1) * marza;
        }
        ui->tableWidget->item(i,1)->setText(QString::number(oddeffective));
        double stake = ui->tableWidget->item(i,2)->text().toDouble();

        if(stake>0 && firstStake==-1){
            //Get Waluta?
             std::string waluta = ((QComboBox*)(ui->tableWidget->cellWidget(i,7)))->currentText().toStdString();
            firstStake = stake;
            if(waluta=="EUR"){
                firstStake = firstStake*this->kursEur;
             }
            FirstOdd = oddeffective;
        }
        if(!CalcOddLay){
         suminvert += 1/oddeffective;
         qDebug()<< 1/oddeffective;
        }
    }
    ui->Sumaodwrotnosci->setText(QString::number(suminvert));
    //Kursy napewno są i mamy odwrotnosci ja bym podzielil to na kroki
    //1.step Wyliczyc Kursy Efektywne dla kazdej w zalesnoci jaki TYP Marzy(TYP,WIN) JEST OK
    //2.step Jezeli mamy Budzet zakladu to dla kazdego kursu wyliczyc stawke + dla kursow lay
    //2.1step Jezeli nie mamy bierzemy pierwszy z brzegu stawkeI do niej uzupelniamy pozostale kursy(wlacznie z lay) inny wzorek
    //Uwaga na stawke w zaleznosci od ustawien liczmy wyjsciowo liczby dla PLN a wyswietlamy wedlug ustawiedobr
    for(int i=0;i<rows;i++){
        //getEffectiveOdd
        double effectiveOdd = ui->tableWidget->item(i,1)->text().toDouble();
        double OddLay = ui->tableWidget->item(i,5)->text().toDouble();
        double kurs =  ui->tableWidget->item(i,0)->text().toDouble();
        bool CalcOddLay = ((QCheckBox*)ui->tableWidget->cellWidget(i,8))->isChecked();
        double commission = ui->tableWidget->item(i,3)->text().toDouble() / 100.0;
        std::string waluta = ((QComboBox*)(ui->tableWidget->cellWidget(i,7)))->currentText().toStdString();
        double stake = 0;
        double libaility =  0;
        if(okbudzet){

            if(LayCount == 1 && rows == 2){

                // ====== BACK + LAY MODEL ======

                int backRow = (layRow == 0 ? 1 : 0);

                double Kb = ui->tableWidget->item(backRow,1)->text().toDouble();
                double Kl = ui->tableWidget->item(layRow,5)->text().toDouble();
                double c  = ui->tableWidget->item(layRow,3)->text().toDouble()/100.0;

                double B = budzet / (1 + (Kb * (Kl - 1)) / (Kl - c));

                double L = (B * Kb) / (Kl - c);

                if(i == layRow){
                    libaility = L;
                    stake = L * (Kl - 1);
                }
                else{
                    stake = B;
                }
            }
            else if(LayCount>1){
                qDebug()<<"Nie obslguuje wiecej nniz lay";
            }
            else{
                stake = ((1/effectiveOdd)/suminvert) * budzet;
            }
        }
        else if(!okbudzet && firstStake!=-1){
            stake = (firstStake * FirstOdd) / effectiveOdd;
            libaility = stake *(kurs-1);

        }
        else{
            qDebug()<<"Brak Stawki oraz budzetu";
            ui->tableWidget->blockSignals(false);
            return;
        }
        if(waluta=="EUR"){
            stake = stake/converplntoeur;
        }

        ui->tableWidget->item(i,2)->setText(QString::number(stake));
        //Liczmy dla Lay jezeli ma kurs
        if(CalcOddLay){
            if(waluta=="EUR"){
                libaility = libaility/converplntoeur;
            }
            ui->tableWidget->item(i,6)->setText(QString::number(libaility));

        }
        if(!okbudzet)
        {
           //ui->BudzetInput->setPlainText(QString::number(budzet));
        }
        ui->tableWidget->blockSignals(false);
    }

}

void MainWindow::addRow()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // ComboBox
    auto comboTyp = new QComboBox();
    comboTyp->addItems({"Bet","Win"});
    auto comboWaluta = new QComboBox();
    comboWaluta->addItems({"PLN","EUR"});
    ui->tableWidget->setItem(row,0,new QTableWidgetItem(""));
    ui->tableWidget->setItem(row,1,new QTableWidgetItem(""));
    ui->tableWidget->setItem(row,2,new QTableWidgetItem(""));
    ui->tableWidget->setItem(row,3,new QTableWidgetItem(""));
    ui->tableWidget->setCellWidget(row, 4, comboTyp);
    ui->tableWidget->setItem(row,5,new QTableWidgetItem(""));
    ui->tableWidget->setItem(row,6,new QTableWidgetItem(""));
    ui->tableWidget->setCellWidget(row,7,comboWaluta);
    ui->tableWidget->setCellWidget(row,8,new QCheckBox());

}

void MainWindow::on_pushButton_2_clicked()
{
    MainWindow::calcResult();
}



void MainWindow::on_kurseur_editingFinished()
{
    QString arg1 = ui->kurseur->text();
    qDebug()<<"Changed Text";
    bool ok = false;
    double value = arg1.toDouble(&ok);
    if(!ok){
        ui->kurseur->setText("0");
        return;
    }
    this->kursEur=value;
}

