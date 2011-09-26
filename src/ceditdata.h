#ifndef CEDITDATA_H
#define CEDITDATA_H

#include "include.h"

class CEditData
{
public:
	static CEditData &getInstance()
	{
		static CEditData ins ;
		return ins ;
	}

	void initialize() ;
	void release() ;

private:
	CEditData() {}
	~CEditData() {}
	CEditData &operator =(CEditData &rhs) ;
} ;

#define gEditData CEditData::getInstance()

#endif // CEDITDATA_H
