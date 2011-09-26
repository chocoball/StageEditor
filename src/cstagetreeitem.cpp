#include "cstagetreeitem.h"

CStageTreeItem::CStageTreeItem(CStageTreeItem *parent)
{
	m_pParent = parent ;
	setType(kType_None) ;
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
	}
}

