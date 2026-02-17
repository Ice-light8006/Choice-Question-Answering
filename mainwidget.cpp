#include "mainwidget.h"
#include "./ui_mainwidget.h"
#include<QDebug>
#include<QWidget>
#include<QPushButton>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qsw = new QStackedWidget(this);

    page1 = new QWidget;
    page2 = new QWidget;
    page3 = new QWidget;

    QPushButton *bt1 = new QPushButton("检测答案",page1);

    qsw->addWidget(page1);
}

MainWidget::~MainWidget()
{
    delete ui;
    delete page1;
    delete page2;
    delete page3;
}


void MainWidget::on_Answer_pushButton_clicked()
{
    this->qsw->show();
    this->qsw->setCurrentWidget(this->page1);

}

