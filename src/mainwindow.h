#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include "cglgameview.h"

namespace Ui {
	class MainWindow ;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0) ;
	~MainWindow() ;

public slots:
	void closeEvent(QCloseEvent *event) ;

signals:
	void sig_keyPress(QKeyEvent *) ;
	void sig_keyRelease(QKeyEvent *) ;

protected:
	void keyPressEvent(QKeyEvent *) ;
	void keyReleaseEvent(QKeyEvent *) ;

private:
	void addUIs() ;
	void restoreSetting() ;
	void writeSetting() ;

private:
	Ui::MainWindow	*ui ;

	QSplitter		*m_pSplitter ;
	CGLGameView		*m_pGlView ;
} ;

#endif // MAINWINDOW_H
