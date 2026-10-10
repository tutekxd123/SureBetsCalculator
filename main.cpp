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

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
using namespace Qt::StringLiterals;
int main(int argc, char *argv[])
{
    QTranslator qtTranslator;
    QString pathToLoadTrans = QString("surebetcalc_%1.ts").arg(QLocale().system().name());
    bool loadedTranslation = qtTranslator.load(pathToLoadTrans);

    QApplication a(argc, argv);
    //Load File? Translator
    if (loadedTranslation){
        a.installTranslator(&qtTranslator);

    }
    MainWindow w;
    w.show();
    return a.exec();
}
