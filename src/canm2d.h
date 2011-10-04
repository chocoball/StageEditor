#ifndef CANM2D_H
#define CANM2D_H

#include <QString>
#include <QDomDocument>
#include <QMatrix4x4>
#include <QDir>
#include "framedata.h"

typedef struct _tagAnmLayer {
	_tagAnmLayer()
	{
		pParentLayer = NULL ;
	}
	~_tagAnmLayer()
	{
		for ( int i = 0 ; i < childPtrs.size() ; i ++ ) {
			delete childPtrs[i] ;
		}
		childPtrs.clear() ;
	}

	FrameData *getFrameDataPtr(int frame)
	{
		for ( int i = 0 ; i < frameDatas.size() ; i ++ ) {
			if ( frame == frameDatas.at(i).frame ) { return &frameDatas[i] ; }
		}
		return NULL ;
	}
	int getMaxFrameNum(bool bRecv = true)
	{
		int ret = 0 ;

		for ( int i = 0 ; i < frameDatas.size() ; i ++ ) {
			if ( ret < frameDatas.at(i).frame ) {
				ret = frameDatas.at(i).frame ;
			}
		}
		if ( bRecv ) {
			for ( int i = 0 ; i < childPtrs.size() ; i ++ ) {
				int tmp = childPtrs[i]->getMaxFrameNum() ;
				if ( ret < tmp ) { ret = tmp ; }
			}
		}
		return ret ;
	}

	FrameData *getFrameDataFromPrevFrame(int frame, bool bRepeat = false)
	{
		int old = frame ;
		frame -- ;
		while ( old != frame ) {
			if ( frame < 0 ) {
				if ( !bRepeat ) { return NULL ; }

				frame += 2048 ;
			}
			FrameData *pData = getFrameDataPtr(frame) ;
			if ( pData ) { return pData ; }
			frame -- ;
		}
		return NULL ;
	}
	FrameData *getFrameDataFromNextFrame(int frame)
	{
		int max = getMaxFrameNum(false) ;

		for ( int i = frame + 1 ; i < max+1 ; i ++ ) {
			FrameData *pData = getFrameDataPtr(i) ;
			if ( pData ) { return pData ; }
		}
		return NULL ;
	}

	FrameData getDisplayFrameData(int frame, bool *bValid = 0)
	{
		FrameData d ;
		FrameData *pPrev = getFrameDataFromPrevFrame(frame+1) ;
		if ( pPrev ) {
			FrameData *pNext = getFrameDataFromNextFrame(frame) ;
			d = pPrev->getInterpolation(pNext, frame) ;
			d.frame = frame ;
			if ( bValid ) { *bValid = true ; }
		}
		else {
			if ( bValid ) { *bValid = false ; }
		}
		return d ;
	}
	QMatrix4x4 getDisplayMatrix(int frame, bool *bValid = 0)
	{
		QMatrix4x4 parent, mat ;
		if ( pParentLayer ) {
			parent = pParentLayer->getDisplayMatrix(frame) ;
		}

		bool valid ;
		FrameData d = getDisplayFrameData(frame, &valid) ;
		if ( valid ) {
			mat = parent * d.getMatrix() ;

			if ( bValid ) { *bValid = true ; }
		}
		else {
			if ( bValid ) { *bValid = false ; }
		}
		return mat ;
	}

	QList<FrameData>				frameDatas ;
	QList<struct _tagAnmLayer *>	childPtrs ;
	struct _tagAnmLayer				*pParentLayer ;
} AnmLayer ;

typedef struct _tagAnmObject {
	_tagAnmObject()
	{
		nLoop = nFps = 0 ;
	}
	~_tagAnmObject()
	{
		qDeleteAll(layerPtrs) ;
		layerPtrs.clear() ;
	}

	int					nLoop ;
	int					nFps ;

	QList<AnmLayer *>	layerPtrs ;
} AnmObject ;

typedef struct {
	QString				path ;
	QSize				origSize ;
	int					no ;
} AnmImage ;

class CAnm2D
{
public:
	CAnm2D() ;
	virtual ~CAnm2D() ;

	void releaseAll() ;
	void renderOpenGL(int nObj, int frame = 0) ;

	QList<AnmImage> &getImages() { return m_images ; }
	virtual bool load(const QString &fileName) = 0 ;

	QRect getRect() { return m_rect ; }

protected:
	void convMat(double *ret, const QMatrix4x4 &mat) ;
	AnmImage *getImage(int no) ;

	void renderOpenGL_Layer(AnmLayer *pLayer, int frame) ;
	void renderOpenGL_FrameData(const FrameData &data, QMatrix4x4 mat) ;
	void drawRect(QRectF rc, QRectF uv, float z, QColor col) ;

	void setRect() ;
	void setRect(AnmLayer *pLayer) ;

	// srcからdestへの相対パスを絶対パスに変換
	QString getAbsolutePath(QString &src, QString &dest)
	{
		QString path = src ;
		if ( path.at(path.count()-1) != '/' ) {
			path.chop(path.count()-path.lastIndexOf("/")-1) ;
		}
		QDir dir(path) ;
		return dir.absoluteFilePath(dest) ;
	}

protected:
	QList<AnmObject *>	m_objPtrs ;
	QList<AnmImage>		m_images ;
	QRect				m_rect ;
} ;


class CAnm2DXml : public CAnm2D
{
public:
	CAnm2DXml() {}
	CAnm2DXml(const QString &fileName) ;
	~CAnm2DXml() ;

	bool load(const QString &fileName) ;

private:
	bool load(const QDomDocument &xml) ;
	bool loadElement(QDomNode node, const int objNum, const int imageNum) ;
	bool loadImage(QDomNode node, AnmImage &data) ;
	bool loadLayers(QDomNode node, AnmObject *pObj) ;
	bool loadLayers(QDomNode node, AnmLayer *pLayer) ;
	bool loadFrameData(QDomNode node, AnmLayer *pLayer, const int frameDataNum) ;

private:
	QString		m_fileName ;
} ;

#endif // CANM2D_H
