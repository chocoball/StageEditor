#ifndef CGLGAMEVIEW_H
#define CGLGAMEVIEW_H

#include <QGLWidget>

class CGLGameView : public QGLWidget
{
	Q_OBJECT
public:
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

	void drawRoot(int type) ;

private:
	QSize		m_drawSize ;
} ;

#endif // CGLGAMEVIEW_H
