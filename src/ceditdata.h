#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QTreeView>
#include <GL/gl.h>
#include "include.h"
#include "cstagetreemodel.h"
#include "cmaplistmodel.h"

typedef struct {
	QString		filePath ;
	GLuint		nTexObj ;
	QSize		origSize ;
} GLTexture ;

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

	void addTexture(QString path, GLuint obj, QSize origSize) ;
	const GLTexture *getTexture(QString path) ;
	QList<GLTexture> &getTextureList() { return m_textures ; }

	CStageTreeModel	*getStageModel()	{ return m_pStageModel ; }
	CMapListModel	*getMapModel()		{ return m_pMapModel ; }

	QModelIndex getStageSelIndex()
	{
		if ( !m_pStageTreeView ) { return QModelIndex() ; }
		return m_pStageTreeView->currentIndex() ;
	}

	kAccessor(QTreeView*, m_pStageTreeView, StageTreeView)
	kAccessor(QSize, m_mapSize, MapSize)

private:
	CEditData()
	{
		m_pStageTreeView = NULL ;
		m_pStageModel = NULL ;
		m_pMapModel = NULL ;
	}

	~CEditData() {}
	CEditData &operator =(CEditData &rhs) ;

	CStageTreeModel		*m_pStageModel ;
	CMapListModel		*m_pMapModel ;
	QList<GLTexture>	m_textures ;
} ;

#define gEditData CEditData::getInstance()

#endif // CEDITDATA_H
