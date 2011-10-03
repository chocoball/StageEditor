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

void CEditData::addTexture(QString path, GLuint obj, QSize origSize)
{
	for ( int i = 0 ; i < m_textures.size() ; i ++ ) {
		if ( m_textures.at(i).filePath == path ) { return ; }
	}
	GLTexture tmp ;
	tmp.filePath = path ;
	tmp.nTexObj = obj ;
	tmp.origSize = origSize ;
	m_textures.append(tmp) ;
}

const GLTexture *CEditData::getTexture(QString path)
{
	for ( int i = 0 ; i < m_textures.size() ; i ++ ) {
		if ( m_textures.at(i).filePath == path ) {
			return &m_textures[i] ;
		}
	}
	return NULL ;
}



