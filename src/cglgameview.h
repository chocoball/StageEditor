#ifndef CGLGAMEVIEW_H
#define CGLGAMEVIEW_H

#include <QGLWidget>

class CGLGameView : public QGLWidget
{
	Q_OBJECT
public:
	explicit CGLGameView(QWidget *parent = 0) ;

signals:

public slots:

protected:
	void initializeGL() ;
	void paintGL() ;
	void resizeGL(int w, int h) ;
	void dragEnterEvent(QDragEnterEvent *event) ;
	void dropEvent(QDropEvent *event) ;
} ;

#endif // CGLGAMEVIEW_H
