#include <QFile>
#include <QStringList>
#include <GL/gl.h>
#include "canm2d.h"
#include "ceditdata.h"

#define kAnmXML_Version			0x01000001		///< バージョン 0x[00][000][000]

#define kAnmXML_Attr_Version	"Version"
#define kAnmXML_Attr_ObjNum		"ObjNum"
#define kAnmXML_Attr_LoopNum	"LoopNum"
#define kAnmXML_Attr_LayerNum	"LayerNum"
#define kAnmXML_Attr_FrameNum	"FrameDataNum"
#define kAnmXML_Attr_ImageNum	"ImageNum"
#define kAnmXML_Attr_No			"No"
#define kAnmXML_Attr_Name		"Name"
#define kAnmXML_Attr_ChildNum	"ChildNum"
#define kAnmXML_Attr_FpsNum		"Fps"

#define kAnmXML_ID_Anm2D		"ANM2D"
#define kAnmXML_ID_Root			"ROOT"
#define kAnmXML_ID_Object		"OBJECT"
#define kAnmXML_ID_Layer		"LAYER"
#define kAnmXML_ID_FrameData	"FRAMEDATA"
#define kAnmXML_ID_Image		"IMAGE"

#ifdef Q_OS_MAC
#include <stdint.h>
#define LP_ADD(p, n) (uint64_t)((uint64_t)((uint64_t *)(p)) + (uint64_t)((uint64_t *)(n)))
#else
#define LP_ADD(p, n) (unsigned int)((unsigned int)(p) + (unsigned int)(n))
#endif


// ------------------------------------------------------
// 基底クラス
// ------------------------------------------------------
CAnm2D::CAnm2D()
{
}

CAnm2D::~CAnm2D()
{
	releaseAll() ;
}

void CAnm2D::releaseAll()
{
	qDeleteAll(m_objPtrs) ;
	m_objPtrs.clear() ;
}

// OpenGL 描画
void CAnm2D::renderOpenGL(int nObj, int frame)
{
	if ( nObj < 0 || nObj >= m_objPtrs.size() ) { return ; }
	AnmObject *p = m_objPtrs[nObj] ;
	for ( int i = 0 ; i < p->layerPtrs.size() ; i ++ ) {
		renderOpenGL_Layer(p->layerPtrs.at(i), frame) ;
	}
}

void CAnm2D::convMat(double *ret, const QMatrix4x4 &mat)
{
	const qreal *p = mat.data() ;
	for ( int i = 0 ; i < 16 ; i ++ ) {
		ret[i] = p[i] ;
	}
}

AnmImage *CAnm2D::getImage(int no)
{
	for ( int i = 0 ; i < m_images.size() ; i ++ ) {
		if ( no == m_images.at(i).no ) { return &m_images[i] ; }
	}
	return NULL ;
}

void CAnm2D::renderOpenGL_Layer(AnmLayer *pLayer, int frame)
{
	bool valid ;
	FrameData d = pLayer->getDisplayFrameData(frame, &valid) ;
	if ( valid ) {
		QMatrix4x4 mat = pLayer->getDisplayMatrix(frame) ;
		renderOpenGL_FrameData(d, mat) ;
	}
}

void CAnm2D::renderOpenGL_FrameData(const FrameData &data, QMatrix4x4 mat)
{
	QRectF rect ;
	QRect uv = data.getRect() ;
	QRectF uvF ;

	AnmImage *pImage = getImage(data.nImage) ;
	if ( !pImage ) { return ; }

	const GLTexture *pTex = gEditData.getTexture(pImage->path) ;
	if ( !pTex ) { return ; }

	glPushMatrix() ;
	{
		double m[16] ;
		convMat(m, mat) ;
		m[14] /= 4096.0 ;
		glMultMatrixd(m) ;

		Vertex v = data.getVertex() ;
		rect.setLeft(v.x0);
		rect.setRight(v.x1) ;
		rect.setTop(v.y0);
		rect.setBottom(v.y1);

		uvF.setLeft((float)uv.left()/pImage->origSize.width());
		uvF.setRight((float)uv.right()/pImage->origSize.width());
		uvF.setTop((float)(pImage->origSize.height()-uv.top())/pImage->origSize.height());
		uvF.setBottom((float)(pImage->origSize.height()-uv.bottom())/pImage->origSize.height());

		glBindTexture(GL_TEXTURE_2D, pTex->nTexObj) ;

		QColor col ;
		col.setRed(   data.rgba[0] );
		col.setGreen( data.rgba[1] );
		col.setBlue(  data.rgba[2] );
		col.setAlpha( data.rgba[3] );

		drawRect(rect, uvF, 0, col) ;
	}
	glPopMatrix() ;
}

void CAnm2D::drawRect(QRectF rc, QRectF uv, float z, QColor col)
{
	glColor4ub(col.red(), col.green(), col.blue(), col.alpha());

	glBegin(GL_TRIANGLE_STRIP) ;
		glTexCoord2f(uv.left(), uv.bottom());
		glVertex3f(rc.left(), rc.bottom(), z) ;
		glTexCoord2f(uv.right(), uv.bottom());
		glVertex3f(rc.right(), rc.bottom(), z) ;
		glTexCoord2f(uv.left(), uv.top());
		glVertex3f(rc.left(), rc.top(), z) ;
		glTexCoord2f(uv.right(), uv.top());
		glVertex3f(rc.right(), rc.top(), z) ;
	glEnd() ;
}



// ------------------------------------------------------
// XML
// ------------------------------------------------------
CAnm2DXml::CAnm2DXml(const QString &fileName) :
	CAnm2D()
{
	m_fileName = fileName ;
	if ( !load(fileName) ) {
		releaseAll();
	}
}

CAnm2DXml::~CAnm2DXml()
{
}

bool CAnm2DXml::load(const QString &fileName)
{
	m_fileName = fileName ;

	QFile file(fileName) ;
	if ( !file.open(QFile::ReadOnly) ) {
		return false ;
	}
	QDomDocument xml ;
	xml.setContent(&file) ;
	if ( !load(xml) ) {
		releaseAll() ;
		return false ;
	}
	return true ;
}

bool CAnm2DXml::load(const QDomDocument &xml)
{
	if ( xml.doctype().name() != kAnmXML_ID_Anm2D ) {
		return false ;
	}

	QDomElement root = xml.documentElement() ;
	if ( root.isNull() ) {
		return false ;
	}
	if ( root.nodeName() != kAnmXML_ID_Root ) {
		return false ;
	}

	QDomNamedNodeMap nodeMap = root.attributes() ;
	if ( nodeMap.isEmpty() ) {
		return false ;
	}
	if ( nodeMap.namedItem(kAnmXML_Attr_Version).isNull() ) {
		return false ;
	}
	int version = nodeMap.namedItem(kAnmXML_Attr_Version).toAttr().value().toInt() ;
	if ( version != kAnmXML_Version ) {
		return false ;
	}

	if ( nodeMap.namedItem(kAnmXML_Attr_ObjNum).isNull() ) {
		return false ;
	}
	if ( nodeMap.namedItem(kAnmXML_Attr_ImageNum).isNull() ) {
		return false ;
	}
	int objNum = nodeMap.namedItem(kAnmXML_Attr_ObjNum).toAttr().value().toInt() ;
	int imageNum = nodeMap.namedItem(kAnmXML_Attr_ImageNum).toAttr().value().toInt() ;

	QDomNode n = root.firstChild() ;
	return loadElement(n, objNum, imageNum) ;
}

bool CAnm2DXml::loadElement(QDomNode node, const int objNum, const int imageNum)
{
	while ( !node.isNull() ) {
		if ( node.nodeName() == kAnmXML_ID_Object ) {	// オブジェクト
			QString name ;
			int layerNum = 0 ;
			int no = 0 ;
			QDomNamedNodeMap nodeMap = node.attributes() ;
			if ( nodeMap.namedItem(kAnmXML_Attr_Name).isNull()
			  || nodeMap.namedItem(kAnmXML_Attr_LayerNum).isNull()
			  || nodeMap.namedItem(kAnmXML_Attr_No).isNull()
			  || nodeMap.namedItem(kAnmXML_Attr_LoopNum).isNull() ) {
				return false ;
			}
			name = nodeMap.namedItem(kAnmXML_Attr_Name).toAttr().value() ;
			layerNum = nodeMap.namedItem(kAnmXML_Attr_LayerNum).toAttr().value().toInt() ;
			no = nodeMap.namedItem(kAnmXML_Attr_No).toAttr().value().toInt() ;
			int loopNum = nodeMap.namedItem(kAnmXML_Attr_LoopNum).toAttr().value().toInt() ;
			int fps = 60 ;
			if ( !nodeMap.namedItem(kAnmXML_Attr_FpsNum).isNull() ) {
				fps = nodeMap.namedItem(kAnmXML_Attr_FpsNum).toAttr().value().toInt() ;
			}
			AnmObject *pObj = new AnmObject ;
			pObj->nLoop = loopNum ;
			pObj->nFps = fps ;

			QDomNode child = node.firstChild() ;
			if ( !loadLayers(child, pObj) ) { return false ; }
			m_objPtrs.append(pObj) ;
		}
		else if ( node.nodeName() == kAnmXML_ID_Image ) {	// イメージ
			QDomNamedNodeMap nodeMap = node.attributes() ;
			if ( nodeMap.namedItem(kAnmXML_Attr_No).isNull() ) {
				return false ;
			}
			int no = nodeMap.namedItem(kAnmXML_Attr_No).toAttr().value().toInt() ;

			AnmImage data ;
			QDomNode child = node.firstChild() ;
			if ( !loadImage(child, data) ) {
				return false ;
			}
			data.no = no ;
			m_images.append(data) ;
		}
		node = node.nextSibling() ;
	}
	return true ;
}

bool CAnm2DXml::loadImage(QDomNode node, AnmImage &data)
{
	while ( !node.isNull() ) {
		if ( node.nodeName() == "FilePath" ) {
			if ( !node.hasChildNodes() ) {
				return false ;
			}
			data.path = node.firstChild().toText().nodeValue() ;
			data.path = getAbsolutePath(m_fileName, data.path) ;
		}
		else if ( node.nodeName() == "Size" ) {
			if ( !node.hasChildNodes() ) {
				return false ;
			}
			QStringList size = node.firstChild().toText().nodeValue().split(" ") ;
			if ( size.size() != 2 ) {
				return false ;
			}
			data.origSize = QSize(size[0].toInt(), size[1].toInt()) ;
		}
		node = node.nextSibling() ;
	}
	return true ;
}

bool CAnm2DXml::loadLayers(QDomNode node, AnmObject *pObj)
{
	while ( !node.isNull() ) {
		if ( node.nodeName() == kAnmXML_ID_Layer ) {
			QDomNamedNodeMap nodeMap = node.attributes() ;
			if ( nodeMap.namedItem(kAnmXML_Attr_Name).isNull()
			  || nodeMap.namedItem(kAnmXML_Attr_FrameNum).isNull()
			  || nodeMap.namedItem(kAnmXML_Attr_ChildNum).isNull() ) {
				return false ;
			}
			QString name ;
			int frameDataNum = 0 ;
			int childNum = 0 ;

			name = nodeMap.namedItem(kAnmXML_Attr_Name).toAttr().value() ;
			frameDataNum = nodeMap.namedItem(kAnmXML_Attr_FrameNum).toAttr().value().toInt() ;
			childNum = nodeMap.namedItem(kAnmXML_Attr_ChildNum).toAttr().value().toInt() ;

			AnmLayer *pLayer = new AnmLayer ;
			pLayer->pParentLayer = NULL ;

			pObj->layerPtrs.append(pLayer) ;

			QDomNode child = node.firstChild() ;
			if ( !loadFrameData(child, pLayer, frameDataNum) ) {
				return false ;
			}
			QDomNode layers = node.firstChild() ;
			if ( !loadLayers(layers, pLayer) ) {
				return false ;
			}
		}
		node = node.nextSibling() ;
	}
	return true ;
}

bool CAnm2DXml::loadLayers(QDomNode node, AnmLayer *pParent)
{
	while ( !node.isNull() ) {
		if ( node.nodeName() == kAnmXML_ID_Layer ) {
			QDomNamedNodeMap nodeMap = node.attributes() ;
			if ( nodeMap.namedItem(kAnmXML_Attr_Name).isNull()
			  || nodeMap.namedItem(kAnmXML_Attr_FrameNum).isNull()
			  || nodeMap.namedItem(kAnmXML_Attr_ChildNum).isNull() ) {
				return false ;
			}
			QString name ;
			int frameDataNum = 0 ;
			int childNum = 0 ;

			name = nodeMap.namedItem(kAnmXML_Attr_Name).toAttr().value() ;
			frameDataNum = nodeMap.namedItem(kAnmXML_Attr_FrameNum).toAttr().value().toInt() ;
			childNum = nodeMap.namedItem(kAnmXML_Attr_ChildNum).toAttr().value().toInt() ;

			AnmLayer *pLayer = new AnmLayer ;
			pLayer->pParentLayer = pParent ;
			pParent->childPtrs.append(pLayer) ;

			QDomNode child = node.firstChild() ;
			if ( !loadFrameData(child, pLayer, frameDataNum) ) {
				return false ;
			}
			QDomNode layers = node.firstChild() ;
			if ( !loadLayers(layers, pLayer) ) {
				return false ;
			}
		}
		node = node.nextSibling() ;
	}
	return true ;
}

bool CAnm2DXml::loadFrameData(QDomNode node, AnmLayer *pLayer, const int frameDataNum)
{
	while ( !node.isNull() ) {
		if ( node.nodeName() == kAnmXML_ID_FrameData ) {
			FrameData data ;

			QDomNode dataNode = node.firstChild() ;
			while ( !dataNode.isNull() ) {
				if ( dataNode.nodeName() == "frame" ) {
					if ( !dataNode.hasChildNodes() ) { return false ; }
					QString frame = dataNode.firstChild().toText().nodeValue() ;
					if ( frame.isEmpty() ) { return false ; }
					data.frame = frame.toInt() ;
				}
				else if ( dataNode.nodeName() == "pos" ) {
					if ( !dataNode.hasChildNodes() ) { return false ; }
					QStringList pos = dataNode.firstChild().toText().nodeValue().split(" ") ;
					if ( pos.size() != 3 ) { return false ; }
					data.pos_x = pos[0].toInt() ;
					data.pos_y = pos[1].toInt() ;
					data.pos_z = pos[2].toInt() ;
				}
				else if ( dataNode.nodeName() == "rot" ) {
					if ( !dataNode.hasChildNodes() ) { return false ; }
					QStringList rot = dataNode.firstChild().toText().nodeValue().split(" ") ;
					if ( rot.size() != 3 ) { return false ; }
					data.rot_x = rot[0].toInt() ;
					data.rot_y = rot[1].toInt() ;
					data.rot_z = rot[2].toInt() ;
				}
				else if ( dataNode.nodeName() == "center" ) {
					if ( !dataNode.hasChildNodes() ) { return false ; }
					QStringList center = dataNode.firstChild().toText().nodeValue().split(" ") ;
					if ( center.size() != 2 ) { return false ; }
					data.center_x = center[0].toInt() ;
					data.center_y = center[1].toInt() ;
				}
				else if ( dataNode.nodeName() == "UV" ) {
					if ( !dataNode.hasChildNodes() ) { return false ; }
					QStringList uv = dataNode.firstChild().toText().nodeValue().split(" ") ;
					if ( uv.size() != 4 ) { return false ; }
					data.left	= uv[0].toInt() ;
					data.top	= uv[1].toInt() ;
					data.right	= uv[2].toInt() ;
					data.bottom	= uv[3].toInt() ;
				}
				else if ( dataNode.nodeName() == "ImageNo" ) {
					if ( !dataNode.hasChildNodes() ) { return false ; }
					QString image = dataNode.firstChild().toText().nodeValue() ;
					if ( image.isEmpty() ) { return false ; }
					data.nImage = image.toInt() ;
				}
				else if ( dataNode.nodeName() == "scale" ) {
					if ( !dataNode.hasChildNodes() ) { return false ; }
					QStringList scale = dataNode.firstChild().toText().nodeValue().split(" ") ;
					if ( scale.size() != 2 ) { return false ; }
					data.fScaleX = scale[0].toFloat() ;
					data.fScaleY = scale[1].toFloat() ;
				}
				else if ( dataNode.nodeName() == "UVAnime" ) {
					if ( !dataNode.hasChildNodes() ) { return false ; }
					QString anime = dataNode.firstChild().toText().nodeValue() ;
					if ( anime.isEmpty() ) { return false ; }
					data.bUVAnime = anime.toInt() ? true : false ;
				}
				else if ( dataNode.nodeName() == "Color" ) {	// 頂点色(after ver 0.1.0)
					QStringList color = dataNode.firstChild().toText().nodeValue().split(" ") ;
					if ( color.size() != 4 ) { return false ; }
					data.rgba[0] = color[0].toInt() ;
					data.rgba[1] = color[1].toInt() ;
					data.rgba[2] = color[2].toInt() ;
					data.rgba[3] = color[3].toInt() ;
				}

				dataNode = dataNode.nextSibling() ;
			}

			pLayer->frameDatas.append(data) ;
		}
		node = node.nextSibling() ;
	}
	return true ;
}


