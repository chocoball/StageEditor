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

	CStageTreeItem *getItem(const QModelIndex &index) const ;
	CStageTreeItem *getItem(int absoluteRow) ;

	QModelIndex addItem(int type, QString name) ;
	void removeItem(const QModelIndex &index) ;

	int getType(const QModelIndex &index) ;
	QModelIndex getIndex(int type) ;

	int getAbsoluteRow(const QModelIndex &index) ;

signals:

public slots:

private:
	void updateIndex(CStageTreeItem *p, int row, QModelIndex parent) ;
	int getAbsoluteRow(CStageTreeItem *pRoot, CStageTreeItem *p, int *pRow) ;
	CStageTreeItem *getItem(CStageTreeItem *pRoot, int row, int *pCurrRow) ;

private:
	CStageTreeItem	*m_pRootItem ;
};

#endif // CSTAGETREEMODEL_H
