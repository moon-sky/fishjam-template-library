// gifMaker.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "gifMaker.h"
#include "GifMakerImpl.h"


//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

IGifMaker::IGifMaker()
{

}
IGifMaker::~IGifMaker()
{

}

IGifMaker* IGifMaker::GetInstance(){
    return new CGifMakerImpl();
}
