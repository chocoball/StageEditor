#include <QDragEnterEvent>
#include <QDebug>
#include "cglgameview.h"
#include "ceditdata.h"
#include "canm2d.h"
#include "util.h"

CGLGameView::CGLGameView(QWidget *parent) :
	QGLWidget(QGLFormat(QGL::AlphaChannel), parent)
{
	setAcceptDrops(true) ;
	m_editMode = kEditMode_None ;
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
	glClearColor(0, 0, 0, 0) ;
	glClear(GL_COLOR_BUFFER_BIT) ;

	glMatrixMode(GL_PROJECTION) ;
	glLoadIdentity() ;
	glOrtho(0, m_drawSize.width(), m_drawSize.height(), 0, -1000, 1000);

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
	m_drawSize.setWidth(w) ;
	m_drawSize.setHeight(h) ;
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

		// TODO:コマンドにする ===================================
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
				QSize origSize = img.size() ;
				util::resizeImage(img) ;
				GLuint nObj = bindTexture(img, GL_TEXTURE_2D, GL_RGBA, QGLContext::InvertedYBindOption) ;
				gEditData.addTexture(list[i].path, nObj, origSize, img.size()) ;
				qDebug() << "path:" << list[i].path << " nObj:" << nObj ;
			}

			QModelIndex index = gEditData.getStageModel()->addItem(CStageTreeItem::kType_Map, name) ;
			CStageTreeItem *pItem = gEditData.getStageModel()->getItem(index) ;
			pItem->setAnim(p) ;
			pItem->setPos(event->pos()) ;
		}
		else {
			if ( !gEditData.getTexture(path) ) {
				QImage img ;
				if ( !img.load(path) ) {
					return ;
				}
				QSize origSize = img.size() ;
				util::resizeImage(img) ;
				GLuint nObj = bindTexture(img, GL_TEXTURE_2D, GL_RGBA, QGLContext::InvertedYBindOption) ;
				gEditData.addTexture(path, nObj, origSize, img.size()) ;
			}

			QModelIndex index = gEditData.getStageModel()->addItem(CStageTreeItem::kType_Map, name) ;
			CStageTreeItem *pItem = gEditData.getStageModel()->getItem(index) ;
			pItem->setImage(path) ;
			pItem->setPos(event->pos()) ;
		}
		update() ;
		// ==================================================

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

void CGLGameView::mousePressEvent(QMouseEvent *event)
{
	m_editMode = kEditMode_None ;
	if ( event->button() == Qt::LeftButton ) {
		CStageTreeItem *pItem ;
		int type[2] = {
			CStageTreeItem::kType_ObjectRoot,
			CStageTreeItem::kType_MapRoot
		} ;
		for ( int i = 0 ; i < 2 ; i ++ ) {
			pItem = isContain(type[i], event->pos()) ;
			if ( pItem ) {
				m_pPressItem = pItem ;

				m_editMode = kEditMode_Normal ;
				m_oldItemPos = m_pPressItem->getPos() ;
				m_oldMousePos = event->pos() ;

				gEditData.getStageTreeView()->setCurrentIndex(m_pPressItem->getIndex()) ;
				event->accept() ;
				update() ;

				emit sig_changePos(m_pPressItem->getPos()) ;
				return ;
			}
		}
	}
	event->ignore() ;
}

void CGLGameView::mouseMoveEvent(QMouseEvent *event)
{
	switch ( m_editMode ) {
		case kEditMode_Normal:
			{
				QPoint pos = event->pos() - m_oldMousePos + m_pPressItem->getPos() ;
				m_pPressItem->setPos(pos) ;

				m_oldMousePos = event->pos() ;
				emit sig_changePos(m_pPressItem->getPos()) ;
			}
			update() ;
			break ;
	}
}

void CGLGameView::mouseReleaseEvent(QMouseEvent *event)
{
Q_UNUSED(event)
	switch ( m_editMode ) {
		case kEditMode_Normal:
			if ( m_oldItemPos != m_pPressItem->getPos() ) {
				QList<QWidget *> widget ;
				widget << this ;
				gEditData.cmd_moveItem(m_pPressItem->getIndex(), m_oldItemPos, widget) ;

				emit sig_changePos(m_pPressItem->getPos()) ;
			}
			break ;
	}
	m_editMode = kEditMode_None ;
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

					glPushMatrix();
					{
						QPoint pos = p->getPos() ;
						glTranslatef((GLfloat)pos.x(), (GLfloat)pos.y(), 0.0f);
						pAnm->renderOpenGL(0) ;

						if ( p->getIndex() == gEditData.getStageSelIndex() ) {
							qDebug() << "rect:" << pAnm->getRect() ;
							drawFrame(pAnm->getRect(), QColor(255, 0, 0)) ;
						}
					}
					glPopMatrix();
				}
				break ;
			case CStageTreeItem::kDrawType_Image:
				{
					const GLTexture *pTex = gEditData.getTexture(p->getImagePath()) ;
					if ( !pTex ) { continue ; }
					QGLWidget::drawTexture(QPointF(p->getPos().x(), p->getPos().y()), pTex->nTexObj) ;
				}
				break ;
		}
	}
}

// ライン描画
void CGLGameView::drawLine(const QPoint &pos0, const QPoint &pos1, const QColor &col)
{
	glColor4ub(col.red(), col.green(), col.blue(), col.alpha());

	glBegin(GL_LINES) ;
	glVertex2f(pos0.x(), pos0.y()) ;
	glVertex2f(pos1.x(), pos1.y()) ;
	glEnd() ;
}

void CGLGameView::drawFrame(const QRect &rect, const QColor &col)
{
	bool bTex = glIsEnabled(GL_TEXTURE_2D) ;
	glDisable(GL_TEXTURE_2D) ;

	drawLine(QPoint(rect.left(), rect.top()), QPoint(rect.left(), rect.bottom()), col);
	drawLine(QPoint(rect.right(), rect.top()), QPoint(rect.right(), rect.bottom()), col);
	drawLine(QPoint(rect.left(), rect.top()), QPoint(rect.right(), rect.top()), col);
	drawLine(QPoint(rect.left(), rect.bottom()), QPoint(rect.right(), rect.bottom()), col);

	if ( bTex ) { glEnable(GL_TEXTURE_2D) ; }
}

CStageTreeItem *CGLGameView::isContain(int type, const QPoint &pos)
{
	CStageTreeModel *pModel = gEditData.getStageModel() ;
	QModelIndex index ;
	CStageTreeItem *pItem ;
	index = pModel->getIndex(type) ;
	pItem = pModel->getItem(index) ;
	if ( index.isValid() && pItem ) {
		return pItem->isContain(pos) ;
	}
	return NULL ;
}


