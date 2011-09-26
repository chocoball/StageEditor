#ifndef CSTAGETREEMODEL_H
#define CSTAGETREEMODEL_H

#include <QAbstractItemModel>
#include "cstagetreeitem.h"

class CStageTreeModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	explicit CStageTreeModel(QObject *parent = 0) ;
	~CStageTreeModel() ;

	QVariant data(const QModelIndex &index, int role) const ;
	int rowCount(const QModelIndex &parent) const ;
	int columnCount(const QModelIndex &parent) const ;
	Qt::ItemFlags flags(const QModelIndex &index) const ;
	bool setData(const QModelIndex &index, const QVariant &value, int role) ;
	bool insertRows(int row, int count, const QModelIndex &parent) ;
	bool removeRows(int row, int count, const QModelIndex &parent) ;
	QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const ;
	QModelIndex parent(const QModelIndex &child) const ;

	Qt::DropActions supportedDropActions() const ;
	QStringList mimeTypes() const ;
	QMimeData *mimeData(const QModelIndexList &indexes) const ;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) ;

	CStageTreeItem *getItem(const QModelIndex &index) ;

signals:

public slots:

private:
	CStageTreeItem	*m_pRootItem ;
};

#endif // CSTAGETREEMODEL_H
