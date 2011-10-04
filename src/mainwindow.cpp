#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include.h"
#include "ceditdata.h"
#include "form_maptab.h"

#define kExecName	"StageEditor"
#define kVersion	"0.0.1"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	gSetting.read() ;
	gEditData.initialize();

	ui->setupUi(this) ;
	ui->centralWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored) ;

	addUIs() ;
	restoreSetting() ;

	setWindowTitle(kExecName) ;
	setFocusPolicy(Qt::StrongFocus) ;
}

MainWindow::~MainWindow()
{
	delete ui ;
}

void MainWindow::slot_stageTreeCustomContextMenu(QPoint pos)
{
	QModelIndex index = m_pStageTree->indexAt(pos) ;
	if ( !index.isValid() ) { return ; }

	int type = gEditData.getStageModel()->getType(index) ;
	switch ( type ) {
		case CStageTreeItem::kType_Map:
			break ;
		case CStageTreeItem::kType_Object:
			break ;
		default:
			return ;
	}
}

void MainWindow::slot_changeMapSize()
{
	m_pGlView->resize(gEditData.getMapSize()) ;
	m_pGlView->update() ;
}

void MainWindow::slot_clickStageTree(QModelIndex)
{
	m_pGlView->update() ;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
Q_UNUSED(event)
	writeSetting() ;
	gEditData.release() ;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	emit sig_keyPress(event) ;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	emit sig_keyRelease(event) ;
}

void MainWindow::addUIs()
{
	QGridLayout *pLayout = new QGridLayout(ui->centralWidget) ;
	m_pSplitter = new QSplitter(this) ;
	pLayout->addWidget(m_pSplitter) ;
	{
		m_pStageTree = new QTreeView(this) ;
		m_pStageTree->setModel(gEditData.getStageModel()) ;
		m_pStageTree->setMinimumSize(100, 300) ;
		m_pStageTree->setHeaderHidden(true) ;
		connect(m_pStageTree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slot_stageTreeCustomContextMenu(QPoint))) ;
		connect(m_pStageTree, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_clickStageTree(QModelIndex))) ;
		gEditData.setStageTreeView(m_pStageTree) ;

		QScrollArea *pScroll = new QScrollArea(this) ;
		pScroll->setWidgetResizable(false) ;
		{
			m_pGlView = new CGLGameView(this) ;
			m_pGlView->resize(1024, 1024);
			m_pGlView->setAcceptDrops(true) ;
			pScroll->setWidget(m_pGlView) ;
		}

		QTabWidget *pTabWidget = new QTabWidget(this) ;
		Form_Maptab *pMapTab = new Form_Maptab(this) ;
		pTabWidget->insertTab(0, pMapTab, "Map") ;
		pTabWidget->insertTab(1, new QWidget(this), "Object") ;

		connect(pMapTab, SIGNAL(sig_changeMapSize()), this, SLOT(slot_changeMapSize())) ;

		m_pSplitter->addWidget(m_pStageTree) ;
		m_pSplitter->addWidget(pScroll) ;
		m_pSplitter->addWidget(pTabWidget) ;
	}
}

void MainWindow::restoreSetting()
{
	m_pSplitter->restoreState(gSetting.getSplitterState()) ;
	m_pSplitter->restoreGeometry(gSetting.getSplitterGeometry()) ;

	restoreState(gSetting.getMainWindowState()) ;
	restoreGeometry(gSetting.getMainWindowGeometry()) ;
}

void MainWindow::writeSetting()
{
	gSetting.setSplitterGeometry(m_pSplitter->saveGeometry()) ;
	gSetting.setSplitterState(m_pSplitter->saveState()) ;

	gSetting.setMainWindowGeometry(saveGeometry()) ;
	gSetting.setMainWindowState(saveState()) ;

	gSetting.write() ;
}
