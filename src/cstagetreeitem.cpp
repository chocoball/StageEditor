#include "cstagetreeitem.h"
#include "ceditdata.h"

CStageTreeItem::CStageTreeItem(CStageTreeItem *parent)
{
	m_pParent = parent ;

	setType(kType_None) ;
	setDrawType(kDrawType_None) ;
	setAnmPtr(NULL) ;
}

CStageTreeItem::CStageTreeItem(int type, QString name, CStageTreeItem *parent)
{
	setType(type) ;
	m_name = name ;
	m_pParent = parent ;

	setDrawType(kDrawType_None) ;
	setAnmPtr(NULL) ;
}

CStageTreeItem::~CStageTreeItem()
{
	qDeleteAll(m_pChildren) ;
}

void CStageTreeItem::insertChild(int row, CStageTreeItem *p)
{
	m_pChildren.insert(row, p) ;
}

void CStageTreeItem::removeChild(int row)
{
	if ( row < 0 || row >= m_pChildren.size() ) { return ; }
	CStageTreeItem *p = m_pChildren.takeAt(row) ;
	delete p ;
}

QVariant CStageTreeItem::data(int role)
{
	switch ( role ) {
		case Qt::EditRole:
		case Qt::DisplayRole:
			return m_name ;
			break ;
		case Qt::UserRole:
			break ;
	}

	return QVariant() ;
}

void CStageTreeItem::setData(QVariant val, int role)
{
	switch ( role ) {
		case Qt::EditRole:
		case Qt::DisplayRole:
			m_name = val.toString() ;
			break ;
		case Qt::UserRole:
			break ;
	}
}

CStageTreeItem *CStageTreeItem::getChild(int type)
{
	for ( int i = 0 ; i < childCount() ; i ++ ) {
		if ( !child(i) ) { continue ; }

		if ( child(i)->getType() == type ) {
			return child(i) ;
		}
	}
	return NULL ;
}

void CStageTreeItem::setAnim(CAnm2DXml *p)
{
	setAnmPtr(p) ;
	setDrawType(kDrawType_Anm) ;
}

void CStageTreeItem::setImage(QString path)
{
	setImagePath(path) ;
	setDrawType(kDrawType_Image) ;
}

CStageTreeItem *CStageTreeItem::isContain(const QPoint &pos)
{
	for ( int i = 0 ; i < childCount() ; i ++ ) {
		CStageTreeItem *p = child(i) ;
		switch ( p->getDrawType() ) {
			case kDrawType_Anm:
				if ( !p->getAnmPtr() ) { continue ; }

				{
					QRect r = p->getAnmPtr()->getRect() ;
					r.moveTo(p->getPos()) ;
					if ( r.contains(pos) ) {
						return p ;
					}
				}
				break ;
			case kDrawType_Image:
				{
					const GLTexture *pTex = gEditData.getTexture(p->getImagePath()) ;
					if ( !pTex ) { continue ; }
					QRect r = QRect(0, 0, pTex->origSize.width(), pTex->origSize.height()) ;
					r.moveTo(p->getPos()) ;
					if ( r.contains(pos) ) {
						return p ;
					}
				}
				break ;
		}
	}
	return NULL ;
}






