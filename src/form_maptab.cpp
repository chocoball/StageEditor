#include <QDebug>
#include "form_maptab.h"
#include "ui_form_maptab.h"

Form_Maptab::Form_Maptab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Maptab)
{
    ui->setupUi(this);
}

Form_Maptab::~Form_Maptab()
{
    delete ui;
}

void Form_Maptab::resizeEvent(QResizeEvent *event)
{
	qDebug() << "Form_Maptab::resizeEvent" << event->size() ;
	qDebug() << "this:" << this->size() ;
	qDebug() << "layoutWidget:" << ui->verticalLayoutWidget->size() ;
	qDebug() << "listView:" << ui->listView->size() ;
}

