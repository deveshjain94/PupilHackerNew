#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string.h>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>

QString nameval;

void write(QString filename, QString nameval, int ageval, QString gend){
QFile mfile(filename);
  if(!mfile.open(QFile::WriteOnly |QFile::Text)){
    qDebug() << "Could not open file for writing";
    return;
  }

  QTextStream out (&mfile);
  out<<nameval;
  out<<"\t";
  out<<ageval;
  out<<"\t";
  out<<gend;

  mfile.flush();
  mfile.close();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pix("../Data/white.jpg");
    ui->label_2->setPixmap(pix);
    QPixmap pix1("../Data/index.png");
    ui->label_3->setPixmap(pix1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString gend;
    nameval=ui->lineEdit_name->text();
    int ageval=ui->age->value();
    if(ui->radioButton_male->isChecked()){
        gend="Male";
    }
    else if(ui->radioButton_female->isChecked()){
        gend="Female";
    }
     QString mfilename = "../Data/data.csv";
     write(mfilename, nameval, ageval, gend);
     system("./pupil");
}

