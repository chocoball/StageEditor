#ifndef CEDITDATA_H
#define CEDITDATA_H

#include <QTreeView>
#include <QGLWidget>
#include <QUndoStack>
#include "include.h"
#include "cstagetreemodel.h"
#include "cmaplistmodel.h"

typedef struct {
	QString		filePath ;
	GLuint		nTexObj ;
	QSize		origSize ;
	QSize		imgSize ;
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

	void addTexture(QString path, GLuint obj, const QSize &origSize, const QSize &imgSize) ;
	const GLTexture *getTexture(QString path) ;
	QList<GLTexture> &getTextureList() { return m_textures ; }

	void cmd_moveItem(const QModelIndex &index, const QPoint &oldPos, QList<QWidget *> &updateWidget) ;
	void cmd_addMapData(const QStringList &fileNames) ;

	CStageTreeModel	*getStageModel()	{ return m_pStageModel ; }
	CMapListModel	*getMapModel()		{ return m_pMapModel ; }
	QUndoStack		*getUndoStack()		{ return m_pUndoStack ; }

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
	QUndoStack			*m_pUndoStack ;
	QList<GLTexture>	m_textures ;
} ;

#define gEditData CEditData::getInstance()

#endif // CEDITDATA_H
