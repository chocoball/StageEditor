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
	gEditData.initialize() ;

	ui->setupUi(this) ;
	ui->centralWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored) ;

	QAction *p = gEditData.getUndoStack()->createUndoAction(this, trUtf8("元に戻す")) ;
	p->setShortcut(QKeySequence::Undo) ;
	ui->menu_edit->addAction(p) ;
	p = gEditData.getUndoStack()->createRedoAction(this, trUtf8("やり直す")) ;
	p->setShortcut(QKeySequence::Redo) ;
	ui->menu_edit->addAction(p) ;

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

void MainWindow::closeEvent(QCloseEvent *event)
{
Q_UNUSED(event)

	// TODO:編集確認

	m_pGameView->close() ;
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
		gEditData.setStageTreeView(m_pStageTree) ;

		m_pGameView = new Form_GameDetail(this) ;

		QTabWidget *pTabWidget = new QTabWidget(this) ;
		Form_Maptab *pMapTab = new Form_Maptab(this) ;
		pTabWidget->insertTab(0, pMapTab, "Map") ;
		pTabWidget->insertTab(1, new QWidget(this), "Object") ;	// TODO:GUI作る

		connect(m_pStageTree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slot_stageTreeCustomContextMenu(QPoint))) ;
		connect(m_pStageTree, SIGNAL(clicked(QModelIndex)), m_pGameView, SLOT(slot_clickStageTree(QModelIndex))) ;
		connect(pMapTab, SIGNAL(sig_changeMapSize()), m_pGameView, SLOT(slot_changeMapSize())) ;

		m_pSplitter->addWidget(m_pStageTree) ;
		m_pSplitter->addWidget(m_pGameView) ;
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
