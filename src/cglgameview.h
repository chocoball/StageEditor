#ifndef CGLGAMEVIEW_H
#define CGLGAMEVIEW_H

#include <QGLWidget>
#include "cstagetreeitem.h"

class CGLGameView : public QGLWidget
{
	Q_OBJECT
public:
	enum {
		kEditMode_None = -1,
		kEditMode_Normal = 1
	} ;

	explicit CGLGameView(QWidget *parent = 0) ;

	void releaseTextures() ;

signals:

public slots:

protected:
	void initializeGL() ;
	void paintGL() ;
	void resizeGL(int w, int h) ;
	void dragEnterEvent(QDragEnterEvent *event) ;
	void dropEvent(QDropEvent *event) ;
	void closeEvent(QCloseEvent *event) ;
	void mousePressEvent(QMouseEvent *event) ;
	void mouseMoveEvent(QMouseEvent *event) ;
	void mouseReleaseEvent(QMouseEvent *event) ;

	void drawRoot(int type) ;
	void drawLine(const QPoint &pos0, const QPoint &pos1, const QColor &col) ;
	void drawFrame(const QRect &rect, const QColor &col) ;

	CStageTreeItem *isContain(int type, const QPoint &pos) ;

private:
	QSize			m_drawSize ;
	QPoint			m_oldItemPos ;
	QPoint			m_oldMousePos ;
	CStageTreeItem	*m_pPressItem ;

	int				m_editMode ;
} ;

#endif // CGLGAMEVIEW_H
