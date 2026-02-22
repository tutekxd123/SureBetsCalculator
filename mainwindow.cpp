#include "mainwindow.h"
#include "./ui_mainwindow.h"


void MainWindow::CalculateOdds(int row, int col){
    double value = ui->tableWidget->item(row,col)->text().toDouble();
    if(value<=0) return;
    bool laychecked = ((QCheckBox*)ui->tableWidget->cellWidget(row,8))->isChecked();
    if(!laychecked) return;
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

void MainWindow::ResetTable(){
    int row = ui->tableWidget->rowCount();
    while(row>0){
        ui->tableWidget->removeRow(0);
        row = ui->tableWidget->rowCount();
    }
    //Remove All Rows

    //Startowe Rows!
    MainWindow::addRow();
    MainWindow::addRow();
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->item(0,3)->setText("12");
    ui->tableWidget->item(1,3)->setText("3");
    //Ustawienie marzy domyslnej
    ((QComboBox*)ui->tableWidget->cellWidget(0,4))->setCurrentText("Bet");
    ((QComboBox*)ui->tableWidget->cellWidget(1,4))->setCurrentText("Win");
    //Ustawienie typu domyslnego
    ((QComboBox*)ui->tableWidget->cellWidget(0,7))->setCurrentText("PLN");
    ((QComboBox*)ui->tableWidget->cellWidget(1,7))->setCurrentText("EUR");
    //ustawienie walut

    //Domyslnie Lay ON
    ((QCheckBox*)ui->tableWidget->cellWidget(1,8))->setChecked(true);
    ui->tableWidget->blockSignals(false);
    //Zmienmy wartości recznie
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
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setHorizontalHeaderLabels({"Kurs","Rzeczywisty", "Stawka","Podatek(%)","Typ","Kurs Lay","Ryzyko Lay","Waluta","Lay?","Zysk"});
    //Tylko jeden Row Select
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    connect(ui->tableWidget, &QTableWidget::itemChanged,this, &MainWindow::onTableItemChanged);
    MainWindow::ResetTable();
    //MainWindow::addRow();


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

QTableWidgetItem* createCenterText(){
    QTableWidgetItem* item = new QTableWidgetItem("");
    item->setTextAlignment(Qt::AlignCenter);
    return item;
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
    double sumastawek = 0;
    for(int i=0;i<rows;i++){
        //getEffectiveOdd
        double effectiveOdd = ui->tableWidget->item(i,1)->text().toDouble();
        double kurs =  ui->tableWidget->item(i,0)->text().toDouble();
        bool CalcOddLay = ((QCheckBox*)ui->tableWidget->cellWidget(i,8))->isChecked();
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
        sumastawek+=stake;
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
    for(int i=0;i<rows;i++){
        double effectiveOdd = ui->tableWidget->item(i,1)->text().toDouble();
        double stake = ui->tableWidget->item(i,2)->text().toDouble();
        //Stawka moze byc w eur wiec konwersja potrzebna bo sumastawek jest w PLN
        std::string waluta = ((QComboBox*)(ui->tableWidget->cellWidget(i,7)))->currentText().toStdString();
        double revenue=0;
        if(waluta=="EUR"){
            stake = stake*this->kursEur;
            revenue = (stake*effectiveOdd)-sumastawek;
            revenue = revenue/this->kursEur;
        }
        else revenue = (stake*effectiveOdd)-sumastawek;
        ui->tableWidget->item(i,9)->setText(QString::number(revenue));
    } //petla jest potrzebna bo suma stawek jest potrzebna ktorej wczesniej nieznalismy
}

void MainWindow::addRow()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    // ComboBox
    auto comboTyp = new QComboBox();
    comboTyp->addItems({"Bet","Win"});
    auto comboWaluta = new QComboBox();
    auto checkbox = new QCheckBox();
    checkbox->setStyleSheet("margin-left:50%; margin-right:50%;");
    comboWaluta->addItems({"PLN","EUR"});
    ui->tableWidget->setItem(row,0,createCenterText());
    ui->tableWidget->setItem(row,1,createCenterText());
    ui->tableWidget->setItem(row,2,createCenterText());
    ui->tableWidget->setItem(row,3,createCenterText());
    ui->tableWidget->setCellWidget(row, 4, comboTyp);
    ui->tableWidget->setItem(row,5,createCenterText());
    ui->tableWidget->setItem(row,6,createCenterText());
    ui->tableWidget->setCellWidget(row,7,comboWaluta);
    ui->tableWidget->setCellWidget(row,8,checkbox);
    ui->tableWidget->setItem(row,9,createCenterText());
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


void MainWindow::on_wyczyscbutton_clicked()
{
    MainWindow::ResetTable();
}

