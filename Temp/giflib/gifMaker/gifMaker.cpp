// gifMaker.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "gifMaker.h"
#include "GifMakerImpl.h"
#include "GifParserImpl.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

IGifMaker::IGifMaker()
{

}
IGifMaker::~IGifMaker()
{

}

IGifParser::IGifParser()
{

}

IGifParser::~IGifParser(){

}

IGifMaker* IGifMaker::GetInstance(){
    return new CGifMakerImpl();
}

IGifParser* IGifParser::GetInstance(){
    return new CGifParserImpl();
}