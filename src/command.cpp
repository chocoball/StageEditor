#include "command.h"
#include "ceditdata.h"

// アイテム移動コマンド
Command_MoveItem::Command_MoveItem(int absoluteRow, const QPoint &oldPos, QList<QWidget *> &updateWidget) :
	QUndoCommand(QObject::trUtf8("移動"))
{
	m_absoluteRow	= absoluteRow ;
	m_oldPos		= oldPos ;
	m_updateWidget	= updateWidget ;

	CStageTreeItem *p = gEditData.getStageModel()->getItem(absoluteRow) ;
	if ( p ) {
		m_newPos = p->getPos() ;
	}
}

void Command_MoveItem::redo()
{
	CStageTreeItem *p = gEditData.getStageModel()->getItem(m_absoluteRow) ;
	if ( !p ) { return ; }
	p->setPos(m_newPos) ;

	for ( int i = 0 ; i < m_updateWidget.size() ; i ++ ) {
		m_updateWidget[i]->update() ;
	}
}

void Command_MoveItem::undo()
{
	CStageTreeItem *p = gEditData.getStageModel()->getItem(m_absoluteRow) ;
	if ( !p ) { return ; }
	p->setPos(m_oldPos) ;

	for ( int i = 0 ; i < m_updateWidget.size() ; i ++ ) {
		m_updateWidget[i]->update() ;
	}
}


// マップデータ追加 コマンド
Command_AddMapData::Command_AddMapData(const QStringList &fileNames) :
	QUndoCommand(QObject::trUtf8("マップデータ追加"))
{
	m_fileNames = fileNames ;
}

void Command_AddMapData::redo()
{
	CMapListModel *pModel = gEditData.getMapModel() ;
	int row = pModel->rowCount() ;
	pModel->insertRows(row, m_fileNames.size()) ;
	for ( int i = 0 ; i < m_fileNames.size() ; i ++ ) {
		QModelIndex index = pModel->index(row+i) ;
		QString fileName = m_fileNames[i] ;
		QString baseName = fileName ;
		QStringList tmp = fileName.split("/") ;
		if ( tmp.size() ) {
			baseName = tmp[tmp.size()-1] ;
		}
		QImage img ;
		img.load(fileName) ;

		pModel->setData(index, baseName, Qt::DisplayRole) ;
		pModel->setData(index, fileName, Qt::UserRole) ;
		pModel->setPixmap(index, QPixmap::fromImage(img));

		m_indexes.append(index) ;
	}
}

void Command_AddMapData::undo()
{
	if ( m_indexes.size() <= 0 ) { return ; }

	CMapListModel *pModel = gEditData.getMapModel() ;
	pModel->removeRows(m_indexes[0].row(), m_indexes.size()) ;

	m_indexes.clear() ;
}




