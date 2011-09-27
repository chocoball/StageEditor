#ifndef CSTAGETREEITEM_H
#define CSTAGETREEITEM_H

#include <QString>
#include <QList>
#include <QVariant>

#include "include.h"

class CStageTreeItem
{
public:
	enum {
		kType_None	= -1,
		kType_Root	= 0,
		kType_MapRoot,
		kType_Map,
		kType_ObjectRoot,
		kType_Object
	} ;

public:
	CStageTreeItem(CStageTreeItem *parent = 0) ;
	CStageTreeItem(int type, QString name, CStageTreeItem *parent = 0) ;
	~CStageTreeItem() ;

	void insertChild(int row, CStageTreeItem *p) ;
	void removeChild(int row) ;

	QVariant data(int role) ;
	void setData(QVariant val, int role) ;

	CStageTreeItem *getChild(int type) ;

	int childCount() { return m_pChildren.size() ; }
	CStageTreeItem *child(int row)
	{
		if ( row < 0 || row >= m_pChildren.size() ) { return NULL ; }
		return m_pChildren[row] ;
	}

	CStageTreeItem *parent() { return m_pParent ; }

	kAccessor(int, m_type, Type)
	kAccessor(QModelIndex, m_index, Index)

private:
	QString					m_name ;

	CStageTreeItem			*m_pParent ;
	QList<CStageTreeItem *>	m_pChildren ;
} ;

#endif // CSTAGETREEITEM_H
