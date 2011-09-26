#-------------------------------------------------
#
# Project created by QtCreator 2011-09-26T14:46:38
#
#-------------------------------------------------

QT			+=	core gui opengl xml

TARGET		=	StageEditor
TEMPLATE	=	app


SOURCES		+=	src/main.cpp		\
				src/mainwindow.cpp	\
				src/csetting.cpp \
    src/ceditdata.cpp \
    src/cstagetreemodel.cpp \
    src/cstagetreeitem.cpp

HEADERS		+=	src/mainwindow.h	\
				src/csetting.h		\
				src/include.h \
    src/ceditdata.h \
    src/cstagetreemodel.h \
    src/cstagetreeitem.h

FORMS		+=	ui/mainwindow.ui	\
				ui/form_mock.ui







