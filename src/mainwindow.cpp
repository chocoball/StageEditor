#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "include.h"

#define kExecName	"StageEditor"
#define kVersion	"0.0.1"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	gSetting.read() ;
	gEditData.initialize();

	ui->setupUi(this) ;
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
}

void MainWindow::restoreSetting()
{
	restoreState(gSetting.getMainWindowState()) ;
	restoreGeometry(gSetting.getMainWindowGeometry()) ;
}

void MainWindow::writeSetting()
{
	gSetting.setMainWindowGeometry(saveGeometry()) ;
	gSetting.setMainWindowState(saveState()) ;

	gSetting.write() ;
}
