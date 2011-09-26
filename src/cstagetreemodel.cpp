#include "cstagetreemodel.h"

CStageTreeModel::CStageTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
	m_pRootItem = new CStageTreeItem(CStageTreeItem::kType_Root, "Root") ;
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
}

QModelIndex CStageTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if ( !hasIndex(row, column, parent) ) { return QModelIndex() ; }
	CStageTreeItem *p = getItem(parent) ;
	CStageTreeItem *pChild = p->child(row) ;
	if ( pChild ) {
		return createIndex(row, column, pChild) ;
	}
	return QModelIndex() ;
}

QModelIndex CStageTreeModel::parent(const QModelIndex &child) const
{
	if ( !child.isValid() ) { return QModelIndex() ; }
	CStageTreeItem *c = getItem(child) ;
	CStageTreeItem *p = c->parent() ;
	return createIndex(p->row(), 0, p) ;
}

Qt::DropActions CStageTreeModel::supportedDropActions() const
{
}

QStringList CStageTreeModel::mimeTypes() const
{
}

QMimeData *CStageTreeModel::mimeData(const QModelIndexList &indexes) const
{
}

bool CStageTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
}

CStageTreeItem *CStageTreeModel::getItem(const QModelIndex &index)
{
	if ( !index.isValid() ) { return m_pRootItem ; }
	return static_cast<CStageTreeItem *>(index.internalPointer()) ;
}
