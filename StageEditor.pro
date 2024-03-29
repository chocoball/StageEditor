#-------------------------------------------------
#
# Project created by QtCreator 2011-09-26T14:46:38
#
#-------------------------------------------------

QT			+=	core gui opengl xml

TARGET		=	StageEditor
TEMPLATE	=	app


SOURCES		+=	src/main.cpp			\
				src/mainwindow.cpp		\
				src/csetting.cpp		\
				src/ceditdata.cpp		\
				src/cstagetreemodel.cpp	\
				src/cstagetreeitem.cpp	\
				src/cglgameview.cpp		\
				src/form_maptab.cpp		\
				src/cmaplistmodel.cpp	\
				src/canm2d.cpp			\
				src/util.cpp			\
				src/form_gamedetail.cpp	\
				src/command.cpp

HEADERS		+=	src/mainwindow.h		\
				src/csetting.h			\
				src/include.h			\
				src/ceditdata.h			\
				src/cstagetreemodel.h	\
				src/cstagetreeitem.h	\
				src/cglgameview.h		\
				src/form_maptab.h		\
				src/cmaplistmodel.h		\
				src/canm2d.h			\
				src/framedata.h			\
				src/util.h				\
				src/form_gamedetail.h	\
				src/command.h

FORMS		+=	ui/mainwindow.ui		\
				ui/form_mock.ui			\
				ui/form_maptab.ui		\
				ui/form_gamedetail.ui

