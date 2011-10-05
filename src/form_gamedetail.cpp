#include <QVBoxLayout>
#include <QDebug>
#include "form_gamedetail.h"
#include "ui_form_gamedetail.h"
#include "include.h"
#include "ceditdata.h"

Form_GameDetail::Form_GameDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_GameDetail)
{
	ui->setupUi(this) ;

	m_pGLView = new CGLGameView(this) ;
	m_pGLView->resize(1024, 1024) ;
	m_pGLView->setAcceptDrops(true) ;
	ui->scrollArea_gameview->setWidget(m_pGLView) ;

	m_pSplitter = new QSplitter(Qt::Vertical, this) ;
	m_pSplitter->addWidget(ui->scrollArea_gameview) ;
	m_pSplitter->addWidget(ui->widget_detail) ;

	QVBoxLayout *pLayout = new QVBoxLayout(this) ;
	pLayout->addWidget(m_pSplitter) ;
	setLayout(pLayout) ;

	m_pSplitter->restoreGeometry(gSetting.getGameSplitterGeometry()) ;
	m_pSplitter->restoreState(gSetting.getGameSplitterState()) ;
	restoreGeometry(gSetting.getGameViewGeometry()) ;

	connect(m_pGLView, SIGNAL(sig_changePos(QPoint)), this, SLOT(slot_changePos(QPoint))) ;
	connect(ui->spinBox_posx, SIGNAL(valueChanged(int)), this, SLOT(slot_changePoxX(int))) ;
	connect(ui->spinBox_posy, SIGNAL(valueChanged(int)), this, SLOT(slot_changePoxY(int))) ;
}

Form_GameDetail::~Form_GameDetail()
{
    delete ui;
}

void Form_GameDetail::slot_changeMapSize()
{
	m_pGLView->resize(gEditData.getMapSize()) ;
	m_pGLView->update() ;
}

void Form_GameDetail::slot_clickStageTree(QModelIndex)
{
	m_pGLView->update() ;
}

void Form_GameDetail::slot_changePos(QPoint pos)
{
	ui->spinBox_posx->setValue(pos.x()) ;
	ui->spinBox_posy->setValue(pos.y()) ;
}

void Form_GameDetail::slot_changePoxX(int val)
{
	QModelIndex index = gEditData.getStageSelIndex() ;
	if ( !index.isValid() ) { return ; }
	CStageTreeItem *pItem = gEditData.getStageModel()->getItem(index) ;

	QPoint pos = pItem->getPos() ;
	if ( pos.x() == val ) { return ; }
	QPoint oldPos = pos ;
	pos.setX(val) ;
	pItem->setPos(pos) ;

	QList<QWidget *> widget ;
	widget << m_pGLView ;
	gEditData.cmd_moveItem(index, oldPos, widget) ;
}

void Form_GameDetail::slot_changePoxY(int val)
{
	QModelIndex index = gEditData.getStageSelIndex() ;
	if ( !index.isValid() ) { return ; }
	CStageTreeItem *pItem = gEditData.getStageModel()->getItem(index) ;

	QPoint pos = pItem->getPos() ;
	if ( pos.y() == val ) { return ; }
	QPoint oldPos = pos ;
	pos.setY(val) ;
	pItem->setPos(pos) ;

	QList<QWidget *> widget ;
	widget << m_pGLView ;
	gEditData.cmd_moveItem(index, oldPos, widget) ;
}

void Form_GameDetail::closeEvent(QCloseEvent *event)
{
	gSetting.setGameViewGeometry(saveGeometry()) ;
	gSetting.setGameSplitterGeometry(m_pSplitter->saveGeometry()) ;
	gSetting.setGameSplitterState(m_pSplitter->saveState()) ;

	event->accept() ;
}

