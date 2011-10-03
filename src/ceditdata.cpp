#include "ceditdata.h"


void CEditData::initialize()
{
	m_pStageModel = new CStageTreeModel() ;
	m_pMapModel = new CMapListModel() ;
}

void CEditData::release()
{
	if ( m_pStageModel ) {
		delete m_pStageModel ;
		m_pStageModel = NULL ;
	}
	if ( m_pMapModel ) {
		delete m_pMapModel ;
		m_pMapModel = NULL ;
	}
}

void CEditData::addTexture(QString path, GLuint obj)
{
	for ( int i = 0 ; i < m_textures.size() ; i ++ ) {
		if ( m_textures.at(i).filePath == path ) { return ; }
	}
	GLTexture tmp ;
	tmp.filePath = path ;
	tmp.nTexObj = obj ;
	m_textures.append(tmp) ;
}

GLuint CEditData::getTexture(QString path)
{
	for ( int i = 0 ; i < m_textures.size() ; i ++ ) {
		if ( m_textures.at(i).filePath == path ) {
			return m_textures[i].nTexObj ;
		}
	}
	return 0 ;
}



