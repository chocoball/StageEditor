#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QTreeView>
#include "include.h"
#include "cstagetreemodel.h"

class CEditData
{
public:
	static CEditData &getInstance()
	{
		static CEditData ins ;
		return ins ;
	}

	void initialize() ;
	void release() ;

	CStageTreeModel *getModel() { return m_pStageModel ; }

	QModelIndex getStageSelIndex()
	{
		if ( !m_pStageTreeView ) { return QModelIndex() ; }
		return m_pStageTreeView->currentIndex() ;
	}

	kAccessor(QTreeView*, m_pStageTreeView, StageTreeView)

private:
	CEditData()
	{
		m_pStageTreeView = NULL ;
		m_pStageModel = NULL ;
	}

	~CEditData()	{}
	CEditData &operator =(CEditData &rhs) ;

	CStageTreeModel	*m_pStageModel ;
} ;

#define gEditData CEditData::getInstance()

#endif // CEDITDATA_H
