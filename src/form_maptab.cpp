#include <QDebug>
#include "form_maptab.h"
#include "ui_form_maptab.h"
#include "ceditdata.h"

Form_Maptab::Form_Maptab(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Form_Maptab)
{
	ui->setupUi(this);

	ui->listView->setModel(gEditData.getMapModel()) ;
	ui->listView->setIconSize(QSize(32, 32)) ;
	ui->listView->setGridSize(QSize(32, 32)) ;
	ui->listView->setDragEnabled(true) ;
	ui->spinBox_mapw->setValue(1024) ;
	ui->spinBox_maph->setValue(1024) ;
	gEditData.setMapSize(QSize(1024, 1024)) ;

	connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(slot_clickedAdd())) ;
	connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(slot_clickedDel())) ;
	connect(ui->spinBox_mapw, SIGNAL(valueChanged(int)), this, SLOT(slot_changeMapW(int))) ;
	connect(ui->spinBox_maph, SIGNAL(valueChanged(int)), this, SLOT(slot_changeMapH(int))) ;

	setLayout(ui->verticalLayout) ;
}

Form_Maptab::~Form_Maptab()
{
	delete ui;
}

void Form_Maptab::slot_clickedAdd()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this,
														  trUtf8("開く"),
														  gSetting.getMapOpenDir(),
														  tr("All Files (*.png *.xml);;PNG Files (*.png);")) ;
	if ( fileNames.size() <= 0 ) { return ; }

	gSetting.setMapOpenDir(fileNames[0]) ;

	gEditData.cmd_addMapData(fileNames) ;
}

void Form_Maptab::slot_clickedDel()
{
	QModelIndex index = ui->listView->currentIndex() ;
	if ( !index.isValid() ) { return ; }

	CMapListModel *pModel = gEditData.getMapModel() ;
	pModel->removeRows(index.row(), 1) ;		// TODO:コマンドにする
}

void Form_Maptab::slot_changeMapW(int val)
{
	QSize size = gEditData.getMapSize() ;
	size.setWidth(val) ;
	gEditData.setMapSize(size) ;

	emit sig_changeMapSize() ;
}

void Form_Maptab::slot_changeMapH(int val)
{
	QSize size = gEditData.getMapSize() ;
	size.setHeight(val) ;
	gEditData.setMapSize(size) ;

	emit sig_changeMapSize() ;
}

void Form_Maptab::resizeEvent(QResizeEvent *)
{

}
