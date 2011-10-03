#include "cmaplistmodel.h"

CMapListModel::CMapListModel(QObject *parent) :
    QStringListModel(parent)
{
}

QVariant CMapListModel::data(const QModelIndex &index, int role) const
{
	if ( !index.isValid() ) { return QVariant() ; }

	switch ( role ) {
		case Qt::DecorationRole:
			return QIcon(m_pixmaps.value(index.row())) ;
		case Qt::UserRole:
			return m_filePaths.value(index.row()) ;
	}

	return QStringListModel::data(index, role) ;
}

bool CMapListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if ( !index.isValid() ) { return false ; }

	switch ( role ) {
		case Qt::UserRole:
			m_filePaths.replace(index.row(), value.toString()) ;
			return true ;
	}

	return QStringListModel::setData(index, value, role) ;
}

void CMapListModel::setPixmap(const QModelIndex &index, const QPixmap &pix)
{
	if ( !index.isValid() ) { return ; }
	m_pixmaps.replace(index.row(), pix) ;
}

bool CMapListModel::insertRows(int row, int count, const QModelIndex &parent)
{
	if ( QStringListModel::insertRows(row, count, parent) ) {
		beginInsertRows(parent, row, row+count-1) ;
		for ( int i = 0 ; i < count ; i ++ ) {
			m_pixmaps.insert(row, QPixmap()) ;
			m_filePaths.insert(row, QString()) ;
		}
		endInsertRows() ;

		return true ;
	}

	return false ;
}

bool CMapListModel::removeRows(int row, int count, const QModelIndex &parent)
{
	if ( QStringListModel::removeRows(row, count, parent) ) {
		beginRemoveRows(parent, row, row+count-1) ;
		for ( int i = 0 ; i < count ; i ++ ) {
			m_pixmaps.removeAt(row) ;
			m_filePaths.removeAt(row) ;
		}
		endRemoveRows() ;
		return true ;
	}
	return false ;
}

QStringList CMapListModel::mimeTypes() const
{
	QStringList types;
	types << "StageEditor/map-data";
	return types;
}

QMimeData *CMapListModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach (QModelIndex index, indexes) {
		if (index.isValid()) {
			stream << data(index, Qt::UserRole).toString() ;
		}
	}

	mimeData->setData("StageEditor/map-data", encodedData);
	return mimeData;
}





