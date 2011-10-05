#ifndef FORM_GAMEDETAIL_H
#define FORM_GAMEDETAIL_H

#include <QWidget>
#include <QSplitter>
#include "cglgameview.h"

namespace Ui {
	class Form_GameDetail ;
}

class Form_GameDetail : public QWidget
{
	Q_OBJECT

public:
	explicit Form_GameDetail(QWidget *parent = 0) ;
	~Form_GameDetail() ;

public slots:
	void slot_changeMapSize() ;
	void slot_clickStageTree(QModelIndex) ;
	void slot_changePos(QPoint pos) ;
	void slot_changePoxX(int val) ;
	void slot_changePoxY(int val) ;

protected:
	void closeEvent(QCloseEvent *event) ;

private:
	Ui::Form_GameDetail *ui ;
	QSplitter			*m_pSplitter ;
	CGLGameView			*m_pGLView ;
} ;

#endif // FORM_GAMEDETAIL_H
