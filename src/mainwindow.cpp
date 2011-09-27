#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include.h"
#include "ceditdata.h"

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
		QTreeView *pTree = new QTreeView(this) ;
		pTree->setModel(gEditData.getModel()) ;
		pTree->setMinimumSize(100, 300) ;
		pTree->setHeaderHidden(true) ;

		QScrollArea *pScroll = new QScrollArea(this) ;
		pScroll->setWidgetResizable(false) ;
		{
			m_pGlView = new CGLGameView(this) ;
			m_pGlView->resize(1024, 1024);
			m_pGlView->setAcceptDrops(true) ;
			pScroll->setWidget(m_pGlView) ;
		}

		QTabWidget *pTabWidget = new QTabWidget(this) ;
		pTabWidget->addTab(new QWidget(this), "Map") ;
		pTabWidget->addTab(new QWidget(this), "Object") ;

		m_pSplitter->addWidget(pTree) ;
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
