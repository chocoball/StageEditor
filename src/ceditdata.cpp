#include "ceditdata.h"
#include "command.h"

void CEditData::initialize()
{
	m_pStageModel	= new CStageTreeModel() ;
	m_pMapModel		= new CMapListModel() ;
	m_pUndoStack	= new QUndoStack() ;
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
	if ( m_pUndoStack ) {
		delete m_pUndoStack ;
		m_pUndoStack = NULL ;
	}
}

void CEditData::addTexture(QString path, GLuint obj, const QSize &origSize, const QSize &imgSize)
{
	for ( int i = 0 ; i < m_textures.size() ; i ++ ) {
		if ( m_textures.at(i).filePath == path ) { return ; }
	}
	GLTexture tmp ;
	tmp.filePath = path ;
	tmp.nTexObj = obj ;
	tmp.origSize = origSize ;
	tmp.imgSize = imgSize ;
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

void CEditData::cmd_moveItem(const QModelIndex &index, const QPoint &oldPos, QList<QWidget *> &updateWidget)
{
	m_pUndoStack->push(new Command_MoveItem(getStageModel()->getAbsoluteRow(index), oldPos, updateWidget)) ;
}

void CEditData::cmd_addMapData(const QStringList &fileNames)
{
	m_pUndoStack->push(new Command_AddMapData(fileNames)) ;
}


