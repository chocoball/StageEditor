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

	connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(slot_clickedAdd())) ;
	connect(ui->pushButton_del, SIGNAL(clicked()), this, SLOT(slot_clickedDel())) ;

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
														  tr("All Files (*);;PNG Files (*.png);")) ;
	if ( fileNames.size() <= 0 ) { return ; }

	gSetting.setMapOpenDir(fileNames[0]) ;

	CMapListModel *pModel = gEditData.getMapModel() ;
	int row = pModel->rowCount() ;
	pModel->insertRows(row, fileNames.size()) ;
	for ( int i = 0 ; i < fileNames.size() ; i ++ ) {
		QModelIndex index = pModel->index(row+i) ;
		QString fileName = fileNames[i] ;
		QString baseName = fileName ;
		QStringList tmp = fileName.split("/") ;
		if ( tmp.size() ) {
			baseName = tmp[tmp.size()-1] ;
		}
		QImage img ;
		img.load(fileName) ;

		pModel->setData(index, baseName, Qt::DisplayRole) ;
		pModel->setData(index, fileName, Qt::UserRole) ;
		pModel->setPixmap(index, QPixmap::fromImage(img));
	}
}

void Form_Maptab::slot_clickedDel()
{
	QModelIndex index = ui->listView->currentIndex() ;
	if ( !index.isValid() ) { return ; }

	CMapListModel *pModel = gEditData.getMapModel() ;
	pModel->removeRows(index.row(), 1) ;
}

