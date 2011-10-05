#include "cstagetreemodel.h"

CStageTreeModel::CStageTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
	m_pRootItem = new CStageTreeItem(CStageTreeItem::kType_Root, "Root") ;
	CStageTreeItem *p ;
	p = new CStageTreeItem(CStageTreeItem::kType_MapRoot, "Map", m_pRootItem) ;
	m_pRootItem->insertChild(0, p) ;
	p = new CStageTreeItem(CStageTreeItem::kType_ObjectRoot, "Object", m_pRootItem) ;
	m_pRootItem->insertChild(1, p) ;

	updateIndex(m_pRootItem, 0, QModelIndex()) ;
}

CStageTreeModel::~CStageTreeModel()
{
	delete m_pRootItem ;
}

QVariant CStageTreeModel::data(const QModelIndex &index, int role) const
{
	CStageTreeItem *p = getItem(index) ;
	return p->data(role) ;
}

int CStageTreeModel::rowCount(const QModelIndex &parent) const
{
	CStageTreeItem *p = getItem(parent) ;
	return p->childCount() ;
}

int CStageTreeModel::columnCount(const QModelIndex &parent) const
{
Q_UNUSED(parent)
	return 1 ;
}

Qt::ItemFlags CStageTreeModel::flags(const QModelIndex &index) const
{
	if ( !index.isValid() ) {
		return Qt::ItemIsEnabled ;
	}
	return Qt::ItemIsEnabled
		 | Qt::ItemIsEditable
		 | Qt::ItemIsSelectable ;
}

bool CStageTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	CStageTreeItem *p = getItem(index) ;
	p->setData(value, role) ;
	return true ;
}

bool CStageTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
	beginInsertRows(parent, row, row+count-1) ;

	CStageTreeItem *p = getItem(parent) ;
	for ( int i = 0 ; i < count ; i ++ ) {
		p->insertChild(row, new CStageTreeItem(p)) ;
	}

	endInsertRows() ;
	return true ;
}

bool CStageTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
	beginRemoveRows(parent, row, row+count-1) ;

	CStageTreeItem *p = getItem(parent) ;
	for ( int i = 0 ; i < count ; i ++ ) {
		p->removeChild(row) ;
	}

	endRemoveRows() ;
	return true ;
}

QModelIndex CStageTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if ( !hasIndex(row, column, parent) ) { return QModelIndex() ; }

	CStageTreeItem *p = getItem(parent) ;
	CStageTreeItem *pChild = p->child(row) ;
	if ( !pChild ) { return QModelIndex() ; }

	return createIndex(row, column, pChild) ;
}

QModelIndex CStageTreeModel::parent(const QModelIndex &child) const
{
	if ( !child.isValid() ) { return QModelIndex() ; }
	CStageTreeItem *c = getItem(child) ;
	CStageTreeItem *p = c->parent() ;
	if ( c == m_pRootItem || p == m_pRootItem ) { return QModelIndex() ; }
	return createIndex(p->getIndex().row(), 0, p) ;
}

CStageTreeItem *CStageTreeModel::getItem(const QModelIndex &index) const
{
	if ( !index.isValid() ) { return m_pRootItem ; }
	return static_cast<CStageTreeItem *>(index.internalPointer()) ;
}

CStageTreeItem *CStageTreeModel::getItem(int absoluteRow)
{
	int row = 0 ;
	return getItem(m_pRootItem, absoluteRow, &row) ;
}

QModelIndex CStageTreeModel::addItem(int type, QString name)
{
	CStageTreeItem *p = NULL ;
	switch ( type ) {
		case CStageTreeItem::kType_Map:
			p = m_pRootItem->getChild(CStageTreeItem::kType_MapRoot) ;
			break ;
		case CStageTreeItem::kType_Object:
			p = m_pRootItem->getChild(CStageTreeItem::kType_ObjectRoot) ;
			break ;
	}
	if ( !p ) {
		return QModelIndex() ;
	}

	int row = p->childCount() ;
	insertRows(row, 1, p->getIndex()) ;

	QModelIndex i = this->index(row, 0, p->getIndex()) ;
	if ( !i.isValid() ) { return QModelIndex() ; }
	p = getItem(i) ;
	p->setIndex(i) ;
	p->setData(name, Qt::DisplayRole) ;
	p->setType(type) ;
	return i ;
}

void CStageTreeModel::removeItem(const QModelIndex &index)
{
	removeRows(index.row(), 1, index.parent()) ;
}

int CStageTreeModel::getType(const QModelIndex &index)
{
	CStageTreeItem *p = getItem(index) ;
	return p->getType() ;
}

QModelIndex CStageTreeModel::getIndex(int type)
{
	for ( int i = 0 ; i < m_pRootItem->childCount() ; i ++ ) {
		CStageTreeItem *p = m_pRootItem->child(i) ;
		if ( p->getType() == type ) { return p->getIndex() ; }
	}
	return QModelIndex() ;
}

int CStageTreeModel::getAbsoluteRow(const QModelIndex &index)
{
	if ( !index.isValid() ) { return 0 ; }
	CStageTreeItem *p = getItem(index) ;
	if ( !p ) { return 0  ; }
	int row = 0 ;
	return getAbsoluteRow(m_pRootItem, p, &row) ;
}



void CStageTreeModel::updateIndex(CStageTreeItem *p, int row, QModelIndex parent)
{
	if ( p->parent() ) {
		p->setIndex(createIndex(row, 0, p)) ;
	}
	else {
		p->setIndex(QModelIndex()) ;
	}
	for ( int i = 0 ; i < p->childCount() ; i ++ ) {
		updateIndex(p->child(i), i, p->getIndex()) ;
	}
}

int CStageTreeModel::getAbsoluteRow(CStageTreeItem *pRoot, CStageTreeItem *p, int *pRow)
{
	if ( pRoot == p ) { return *pRow ; }
	for ( int i = 0 ; i < pRoot->childCount() ; i ++ ) {
		*pRow += 1 ;
		int tmp = getAbsoluteRow(pRoot->child(i), p, pRow) ;
		if ( tmp ) { return tmp ; }
	}
	return 0 ;
}

CStageTreeItem *CStageTreeModel::getItem(CStageTreeItem *pRoot, int row, int *pCurrRow)
{
	if ( row == *pCurrRow ) { return pRoot ; }
	for ( int i = 0 ; i < pRoot->childCount() ; i ++ ) {
		*pCurrRow += 1 ;
		CStageTreeItem *p = getItem(pRoot->child(i), row, pCurrRow) ;
		if ( p ) { return p ; }
	}
	return NULL ;
}






