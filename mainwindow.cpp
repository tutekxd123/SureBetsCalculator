#include "mainwindow.h"
#include "./ui_mainwindow.h"


void MainWindow::CalculateOdds(int row, int col){
    double value = ui->tableWidget->item(row,col)->text().toDouble();
    if(value<=0) return;
    bool laychecked = ((QCheckBox*)ui->tableWidget->cellWidget(row,COL_LAY))->isChecked();
    if(!laychecked) return;
    double newvalue = value/(value-1);
    int coltochange = col==0? 5:0;
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->item(row,coltochange)->setText(QString::number(newvalue));
    ui->tableWidget->blockSignals(false);
}
std::string FindAndReplace(const std::string& stringtoparse,const std::string& stringtofind, const std::string replacestring){
    size_t pos = stringtoparse.find(',');
    std::string result;
    if(pos==std::string::npos){
        result = stringtoparse;
        return result;
    }
    result = stringtoparse.substr(0,pos)+replacestring+stringtoparse.substr(pos+stringtofind.size());
    return result;
}
double MainWindow::RoundTo(double number, double roundto){
    double result = number;
    result = result/roundto;
    result = round(result);
    result = result*roundto;
    return result;
}
void MainWindow::TryParse(int row,int col){
    auto item = ui->tableWidget->item(row,col);
    if(item==nullptr){
        return;
    }
    std::string stringtoparse = ui->tableWidget->item(row,col)->text().toStdString();
    std::string result = FindAndReplace(stringtoparse,",",".");
    ui->tableWidget->item(row,col)->setText(QString::fromStdString(result));
    return;
}
void MainWindow::onTableItemChanged(QTableWidgetItem* item){
    if(!item) return;

    int col = item->column();
    int row = item->row();
    if(col==0 || col== 5 || col==2){
        TryParse(row,col);
    }
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

    //Init Tows
    MainWindow::addRow();
    MainWindow::addRow();
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->item(0,3)->setText("12");
    ui->tableWidget->item(1,3)->setText("3");
    //Sets default Commision
    ((QComboBox*)ui->tableWidget->cellWidget(0,COL_TYPE_COMMISSION))->setCurrentText("Bet");
    ((QComboBox*)ui->tableWidget->cellWidget(1,COL_TYPE_COMMISSION))->setCurrentText("Win");
    //Set default type Commision
    ((QComboBox*)ui->tableWidget->cellWidget(0,COL_CURRENCY))->setCurrentText("PLN");
    ((QComboBox*)ui->tableWidget->cellWidget(1,COL_CURRENCY))->setCurrentText("EUR");
    //Set Currency

    ((QCheckBox*)ui->tableWidget->cellWidget(1,COL_LAY))->setChecked(true);
    ui->tableWidget->blockSignals(false);
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,this, &MainWindow::updateKurs);

    manager->get(QNetworkRequest(QUrl("https://api.nbp.pl/api/exchangerates/rates/a/EUR/?format=JSON")));
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setHorizontalHeaderLabels({"Odd","Effective", "Stake","Commision(%)","Type","Odd Lay","Libaility Lay","Currency","Lay?","Revenue"});

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
        qDebug() << "Error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    qDebug() << "Response:" << data;
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
    ui->tableWidget->blockSignals(true); //Block Event Listener
    double suminvert =0;
    double firstStake = -1;
    double FirstOdd = -1;
    bool okbudzet = false;
    double roundTo = ui->roundto->text().toDouble();
    double budzet = ui->BudzetInput->text().toDouble(&okbudzet);
    double converplntoeur = this->kursEur;
    int LayCount = 0;
    int layRow = -1;
    if(roundTo<=0){
        qDebug()<<"Error roundnumber";
        ui->tableWidget->blockSignals(false);
        return;
    }
    if(converplntoeur<1){
        qDebug()<<"Error EUR KURS";
        ui->tableWidget->blockSignals(false);
        return;
    }
    auto rows = ui->tableWidget->rowCount();
    for(int i=0;i<rows;i++){
        bool isLay = ((QCheckBox*)ui->tableWidget->cellWidget(i,COL_LAY))->isChecked();
        if(isLay){
            LayCount++;
            layRow = i;
        }
        double marza = ui->tableWidget->item(i,COL_COMMISION)->text().toDouble();
        double commission = marza/ 100.0;
        marza = (100-marza)/100; //marza jest odwrotonoscia!
        double kurs =  ui->tableWidget->item(i,COL_ODD)->text().toDouble();
        double OddLay = ui->tableWidget->item(i,COL_ODD_LAY)->text().toDouble();
        if(kurs<=0){
            qDebug()<<"Odd is incorrect";
            ui->tableWidget->blockSignals(false);
            return;
        }
        bool CalcOddLay = ((QCheckBox*)ui->tableWidget->cellWidget(i,COL_LAY))->isChecked();
        if(OddLay>0 && CalcOddLay){
            qDebug()<<1/(OddLay-commission);
            suminvert += 1/(OddLay-commission);
        }
        //Mozna juz tutaj step nr.1
        QComboBox* marzabox =  (QComboBox*)ui->tableWidget->cellWidget(i,COL_TYPE_COMMISSION);
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
        ui->tableWidget->item(i,COL_ODD_EFFECTIVE)->setText(QString::number(oddeffective));
        double stake = ui->tableWidget->item(i,COL_ODD_STAKE)->text().toDouble();

        if(stake>0 && firstStake==-1){
            //Get Waluta?
             std::string waluta = ((QComboBox*)(ui->tableWidget->cellWidget(i,COL_CURRENCY)))->currentText().toStdString();
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
    //ui->Sumaodwrotnosci->setText(QString::number(suminvert));
    //Odds is ready
    //
    //1.step: Calc Effective Odds including type of commision
    //2.step: If we have a betting budget, calculate the stake for each odd, including lay odds
    //2.1step If we don't have pick a first stake and calc betting budget using other mathematical formulas
    //Last Convert All Stake to target Currency
    double sumastawek = 0;
    for(int i=0;i<rows;i++){
        //getEffectiveOdd
        double effectiveOdd = ui->tableWidget->item(i,COL_ODD_EFFECTIVE)->text().toDouble();
        double kurs =  ui->tableWidget->item(i,COL_ODD)->text().toDouble();
        bool CalcOddLay = ((QCheckBox*)ui->tableWidget->cellWidget(i,COL_LAY))->isChecked();
        std::string waluta = ((QComboBox*)(ui->tableWidget->cellWidget(i,COL_CURRENCY)))->currentText().toStdString();
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
                qDebug()<<"Program doesn't support more than 1 lay";
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
            qDebug()<<"Empty Stakes and betting budget";
            ui->tableWidget->blockSignals(false);
            return;
        }
        sumastawek+=stake;
        if(waluta=="EUR"){
            stake = stake/converplntoeur;
        }
        //Round To
        //stake = MainWindow::RoundTo(stake,roundTo);

        ui->tableWidget->item(i,COL_ODD_STAKE)->setText(QString::number(stake));
        //Liczmy dla Lay jezeli ma kurs
        if(CalcOddLay){
            if(waluta=="EUR"){
                libaility = libaility/converplntoeur;
            }
            //libaility = MainWindow::RoundTo(libaility,roundTo);
            ui->tableWidget->item(i,COL_LIBALITY_LAY)->setText(QString::number(libaility));

        }
        if(!okbudzet)
        {
           //ui->BudzetInput->setPlainText(QString::number(budzet));
        }
        ui->tableWidget->blockSignals(false);
    }
    for(int i=0;i<rows;i++){
        double effectiveOdd = ui->tableWidget->item(i,COL_ODD_EFFECTIVE)->text().toDouble();
        double stake = ui->tableWidget->item(i,COL_ODD_EFFECTIVE)->text().toDouble();
        double libality =  ui->tableWidget->item(i,COL_LIBALITY_LAY)->text().toDouble();
        std::string waluta = ((QComboBox*)(ui->tableWidget->cellWidget(i,COL_CURRENCY)))->currentText().toStdString();
        double revenue=0;
        if(waluta=="EUR"){
            stake = stake*this->kursEur;
            revenue = (stake*effectiveOdd)-sumastawek;
            revenue = revenue/this->kursEur;
        }
        else revenue = (stake*effectiveOdd)-sumastawek;
        ui->tableWidget->item(i,COL_REVENUE)->setText(QString::number(revenue));
        //Zakraglsmy Stake dla kazdego
        stake = MainWindow::RoundTo(stake,roundTo);
        ui->tableWidget->item(i,COL_ODD_STAKE)->setText(QString::number(stake));
        if(libality>0){
            libality = MainWindow::RoundTo(libality,roundTo);
            ui->tableWidget->item(i,COL_LIBALITY_LAY)->setText(QString::number(libality));
        }
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
    auto checkbox = new QCheckBox();
    checkbox->setStyleSheet("margin-left:50%; margin-right:50%;");
    comboWaluta->addItems({"PLN","EUR"});
    ui->tableWidget->setItem(row,COL_ODD,createCenterText());
    ui->tableWidget->setItem(row,COL_ODD_EFFECTIVE,createCenterText());
    ui->tableWidget->setItem(row,COL_ODD_STAKE,createCenterText());
    ui->tableWidget->setItem(row,COL_COMMISION,createCenterText());
    ui->tableWidget->setCellWidget(row, COL_TYPE_COMMISSION, comboTyp);
    ui->tableWidget->setItem(row,COL_ODD_LAY,createCenterText());
    ui->tableWidget->setItem(row,COL_LIBALITY_LAY,createCenterText());
    ui->tableWidget->setCellWidget(row,COL_CURRENCY,comboWaluta);
    ui->tableWidget->setCellWidget(row,COL_LAY,checkbox);
    ui->tableWidget->setItem(row,COL_REVENUE,createCenterText());
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


void MainWindow::on_BudzetInput_editingFinished()
{
    //Try parse :D
    std::string currtext = ui->BudzetInput->text().toStdString();
    std::string result = FindAndReplace(currtext,",",".");
    ui->BudzetInput->setText(QString::fromStdString(result));
}




void MainWindow::on_roundto_editingFinished()
{
    std::string currtext = ui->roundto->text().toStdString();
    std::string result = FindAndReplace(currtext,",",".");
    ui->roundto->setText(QString::fromStdString(result));
}

