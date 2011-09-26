#ifndef CEDITDATA_H
#define CEDITDATA_H

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

private:
	CEditData()		{}
	~CEditData()	{}
	CEditData &operator =(CEditData &rhs) ;

	CStageTreeModel	*m_pStageModel ;
} ;

#define gEditData CEditData::getInstance()

#endif // CEDITDATA_H
