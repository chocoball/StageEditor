#ifndef COMMAND_H
#define COMMAND_H

#include <QUndoCommand>
#include <QPoint>
#include <QModelIndex>
#include <QStringList>

// アイテム移動 コマンド
class Command_MoveItem : public QUndoCommand
{
public:
	Command_MoveItem(int absoluteRow, const QPoint &oldPos, QList<QWidget *> &updateWidget) ;

	void redo() ;
	void undo() ;

private:
	int					m_absoluteRow ;
	QPoint				m_oldPos ;
	QPoint				m_newPos ;
	QList<QWidget *>	m_updateWidget ;
} ;

// マップデータ追加 コマンド
class Command_AddMapData : public QUndoCommand
{
public:
	Command_AddMapData(const QStringList &fileNames) ;

	void redo() ;
	void undo() ;

private:
	QStringList			m_fileNames ;
	QModelIndexList		m_indexes ;
} ;

#endif // COMMAND_H
