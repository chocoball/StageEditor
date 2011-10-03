#ifndef CMAPLISTMODEL_H
#define CMAPLISTMODEL_H

#include <QStringListModel>
#include <QMimeData>

class CMapListModel : public QStringListModel
{
	Q_OBJECT
public:
	explicit CMapListModel(QObject *parent = 0) ;

	QVariant data(const QModelIndex &index, int role) const ;
	bool setData(const QModelIndex &index, const QVariant &value, int role) ;
	void setPixmap(const QModelIndex &index, const QPixmap &pix) ;
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) ;
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) ;

	QStringList mimeTypes() const ;
	QMimeData *mimeData(const QModelIndexList &indexes) const ;

signals:

public slots:

private:
	QList<QPixmap>	m_pixmaps ;
	QStringList		m_filePaths ;
} ;

#endif // CMAPLISTMODEL_H
