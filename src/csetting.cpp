#include "csetting.h"

#define kKeyGeometry	"geometry"
#define kKeyState		"state"

void CSetting::read()
{
	QSettings settings(getFilePath(), getFileFormat()) ;

	settings.beginGroup("MainWindow") ;
	setMainWindowGeometry(settings.value(kKeyGeometry).toByteArray()) ;
	setMainWindowState(settings.value(kKeyState).toByteArray()) ;
	settings.endGroup() ;
}

void CSetting::write()
{
	QSettings settings(getFilePath(), getFileFormat()) ;

	settings.beginGroup("MainWindow") ;
	settings.setValue(kKeyGeometry, getMainWindowGeometry()) ;
	settings.setValue(kKeyState, getMainWindowState()) ;
	settings.endGroup() ;
}
