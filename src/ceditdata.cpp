#include "ceditdata.h"


void CEditData::initialize()
{
	m_pStageModel = new CStageTreeModel() ;
}

void CEditData::release()
{
	if ( m_pStageModel ) {
		delete m_pStageModel ;
		m_pStageModel = NULL ;
	}
}

