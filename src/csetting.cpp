#include "csetting.h"

#define kKeyMapOpenDir	"map_open_dir"
#define kKeyGeometry	"geometry"
#define kKeyState		"state"

void CSetting::read()
{
	QSettings settings(getFilePath(), getFileFormat()) ;

	settings.beginGroup("Global") ;
	setMapOpenDir(settings.value(kKeyMapOpenDir).toString()) ;
	settings.endGroup() ;

	settings.beginGroup("MainWindow") ;
	setMainWindowGeometry(settings.value(kKeyGeometry).toByteArray()) ;
	setMainWindowState(settings.value(kKeyState).toByteArray()) ;
	settings.endGroup() ;

	settings.beginGroup("Splitter") ;
	setSplitterGeometry(settings.value(kKeyGeometry).toByteArray()) ;
	setSplitterState(settings.value(kKeyState).toByteArray()) ;
	settings.endGroup() ;

	settings.beginGroup("GameView") ;
	setGameViewGeometry(settings.value(kKeyGeometry).toByteArray()) ;
	settings.endGroup() ;

	settings.beginGroup("GameViewSplitter") ;
	setGameSplitterGeometry(settings.value(kKeyGeometry).toByteArray()) ;
	setGameSplitterState(settings.value(kKeyState).toByteArray()) ;
	settings.endGroup() ;
}

void CSetting::write()
{
	QSettings settings(getFilePath(), getFileFormat()) ;

	settings.beginGroup("Global") ;
	settings.setValue(kKeyMapOpenDir, getMapOpenDir()) ;
	settings.endGroup() ;

	settings.beginGroup("MainWindow") ;
	settings.setValue(kKeyGeometry, getMainWindowGeometry()) ;
	settings.setValue(kKeyState, getMainWindowState()) ;
	settings.endGroup() ;

	settings.beginGroup("Splitter") ;
	settings.value(kKeyGeometry, getSplitterGeometry()) ;
	settings.setValue(kKeyState, getSplitterState()) ;
	settings.endGroup() ;

	settings.beginGroup("GameView") ;
	settings.setValue(kKeyGeometry, getGameViewGeometry()) ;
	settings.endGroup() ;

	settings.beginGroup("GameViewSplitter") ;
	settings.setValue(kKeyGeometry, getGameSplitterGeometry()) ;
	settings.setValue(kKeyState, getGameSplitterState()) ;
	settings.endGroup() ;
}
