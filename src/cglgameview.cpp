#include "cglgameview.h"

CGLGameView::CGLGameView(QWidget *parent) :
	QGLWidget(QGLFormat(QGL::AlphaChannel), parent)
{
}

void CGLGameView::initializeGL()
{
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

	glEnable(GL_ALPHA_TEST) ;
	glAlphaFunc(GL_GREATER, 0) ;

	glEnable(GL_DEPTH_TEST) ;
}

void CGLGameView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;
}

void CGLGameView::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h) ;
}

void CGLGameView::dragEnterEvent(QDragEnterEvent *event)
{
Q_UNUSED(event)
}


void CGLGameView::dropEvent(QDropEvent *event)
{
Q_UNUSED(event)
}

