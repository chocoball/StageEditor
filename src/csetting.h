#ifndef CSETTING_H
#define CSETTING_H

#include <QtGui>

#include "include.h"

class CSetting
{
public:
	static CSetting &getInstance()
	{
		static CSetting ins ;
		return ins ;
	}

	void read() ;
	void write() ;

	QString getFilePath() { return QString(qApp->applicationDirPath() + "/settnig.ini") ; }
	QSettings::Format getFileFormat() { return QSettings::IniFormat ; }

	kAccessor(QByteArray,	m_mainWindowState,		MainWindowState)
	kAccessor(QByteArray,	m_mainWindowGeometry,	MainWindowGeometry)
	kAccessor(QByteArray,	m_splitterState,		SplitterState)
	kAccessor(QByteArray,	m_splitterGeometry,		SplitterGeometry)
	kAccessor(QString,		m_mapOpenDir,			MapOpenDir)

private:
	CSetting() {}
	~CSetting() {}
	CSetting &operator =(CSetting &rhs) ;
} ;

#define gSetting CSetting::getInstance()

#endif // CSETTING_H
