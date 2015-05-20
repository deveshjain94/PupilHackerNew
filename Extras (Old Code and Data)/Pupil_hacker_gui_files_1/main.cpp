#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QString>
#include <QDebug>
#include<QTextStream>

void read(QString filename){
QFile mfile(filename);

  if(!mfile.open(QFile::ReadOnly |QFile::Text)){
    qDebug() << "Could not open file for reading";
    return;
  }

  QTextStream in (&mfile);
  QString mText =in.readAll();

  qDebug() << mText;

  mfile.flush();
  mfile.close();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

