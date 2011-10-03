#include <QDragEnterEvent>
#include "cglgameview.h"
#include "ceditdata.h"
#include "canm2d.h"

CGLGameView::CGLGameView(QWidget *parent) :
	QGLWidget(QGLFormat(QGL::AlphaChannel), parent)
{
	setAcceptDrops(true) ;
}

void CGLGameView::releaseTextures()
{
	QList<GLTexture> &list = gEditData.getTextureList() ;
	for ( int i = 0 ; i < list.size() ; i ++ ) {
		glDeleteTextures(1, &list[i].nTexObj) ;
	}
	list.clear() ;
}

void CGLGameView::initializeGL()
{
	glEnable(GL_BLEND) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) ;

	glEnable(GL_ALPHA_TEST) ;
	glAlphaFunc(GL_GREATER, 0) ;
}

void CGLGameView::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) ;

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	glOrtho(-512/2, 512/2, 512/2, -512/2, -1000, 1000);

	glMatrixMode(GL_MODELVIEW) ;
	glLoadIdentity() ;

	glEnable(GL_TEXTURE_2D) ;

	drawRoot(CStageTreeItem::kType_MapRoot) ;
	drawRoot(CStageTreeItem::kType_ObjectRoot) ;

	glDisable(GL_TEXTURE_2D) ;
	glBindTexture(GL_TEXTURE_2D, 0) ;
}

void CGLGameView::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h) ;
}

void CGLGameView::dragEnterEvent(QDragEnterEvent *event)
{
	if ( event->mimeData()->hasFormat("StageEditor/map-data") ) {
		event->accept() ;
	}
	else {
		event->ignore() ;
	}
}


void CGLGameView::dropEvent(QDropEvent *event)
{
	if ( event->mimeData()->hasFormat("StageEditor/map-data") ) {
		QString path ;
		QByteArray itemData = event->mimeData()->data("StageEditor/map-data");
		QDataStream stream( &itemData, QIODevice::ReadOnly ) ;
		stream >> path ;

		QString name = path ;
		QStringList tmp = path.split("/") ;
		if ( tmp.size() > 0 ) {
			name = tmp[tmp.size()-1] ;
		}

		if ( path.indexOf(".xml") > 0 ) {
			CAnm2DXml *p = new CAnm2DXml() ;
			if ( !p->load(path) ) {
				delete p ;
				return ;
			}
			QList<AnmImage> &list = p->getImages() ;
			for ( int i = 0 ; i < list.size() ; i ++ ) {
				if ( gEditData.getTexture(list[i].path) ) { continue ; }

				QImage img ;
				if ( !img.load(list[i].path) ) {
					delete p ;
					return ;
				}
				GLuint nObj = bindTexture(img, GL_TEXTURE_2D, GL_RGBA, QGLContext::InvertedYBindOption) ;
				gEditData.addTexture(list[i].path, nObj) ;
			}

			QModelIndex index = gEditData.getStageModel()->addItem(CStageTreeItem::kType_Map, name) ;
			CStageTreeItem *pItem = gEditData.getStageModel()->getItem(index) ;
			pItem->setAnim(p) ;
		}
		else {
			QModelIndex index = gEditData.getStageModel()->addItem(CStageTreeItem::kType_Map, name) ;
			CStageTreeItem *pItem = gEditData.getStageModel()->getItem(index) ;
			pItem->setImage(path) ;
		}

		event->accept() ;
		return ;
	}

	event->ignore() ;
}

void CGLGameView::closeEvent(QCloseEvent *event)
{
	releaseTextures() ;
	event->accept() ;
}

void CGLGameView::drawRoot(int type)
{
	QModelIndex index = gEditData.getStageModel()->getIndex(type) ;
	if ( !index.isValid() ) { return ; }

	CStageTreeItem *pRoot = gEditData.getStageModel()->getItem(index) ;
	for ( int i = 0 ; i < pRoot->childCount() ; i ++ ) {
		CStageTreeItem *p = pRoot->child(i) ;
		switch ( p->getDrawType() ) {
			case CStageTreeItem::kDrawType_Anm:
				{
					CAnm2D *pAnm = p->getAnmPtr() ;

					pAnm->renderOpenGL(0) ;
				}
				break ;
			case CStageTreeItem::kDrawType_Image:
				break ;
		}
	}
}




