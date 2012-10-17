//------------------------------------------------------------------------------
// File: TextType.cpp
//
// Desc: DirectShow sample code - implementation of CTextMediaType class.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "TextMediaType.h"

#define DETECT_OTHER_GUID_NAMES

#ifdef DETECT_OTHER_GUID_NAMES


GUID_STRING_ENTRY g_OtherGuidNames[] = {
    #define OUR_GUID_ENTRY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
        { #name, { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } } },

    // MPEG4 AAC
    // wFormatTag   0x0278
    // Format Block [WAVEFORMAT_EX][ES_ID(16bit)]
    // MEDIATYPE_Audio
    // MEDIASUBTYPE_SonyMpeg4AAC
    OUR_GUID_ENTRY(MEDIASUBTYPE_SonyMpeg4AAC,
        0x00000278, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	
	//以下内容来自 <wmsdkidl.h>

	// 00000000-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_Base 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_Base, 
	0x00000000, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 73646976-0000-0010-8000-00AA00389B71  'vids' == WMMEDIATYPE_Video 
	OUR_GUID_ENTRY(WMMEDIATYPE_Video, 
		0x73646976, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// e436eb78-524f-11ce-9f53-0020af0ba770            MEDIASUBTYPE_RGB1 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_RGB1, 
		0xe436eb78, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
	// e436eb79-524f-11ce-9f53-0020af0ba770            MEDIASUBTYPE_RGB4 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_RGB4, 
		0xe436eb79, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
	// e436eb7a-524f-11ce-9f53-0020af0ba770            MEDIASUBTYPE_RGB8 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_RGB8, 
		0xe436eb7a, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
	// e436eb7b-524f-11ce-9f53-0020af0ba770            MEDIASUBTYPE_RGB565 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_RGB565, 
		0xe436eb7b, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
	// e436eb7c-524f-11ce-9f53-0020af0ba770            MEDIASUBTYPE_RGB555 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_RGB555, 
		0xe436eb7c, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
	// e436eb7d-524f-11ce-9f53-0020af0ba770            MEDIASUBTYPE_RGB24 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_RGB24, 
		0xe436eb7d, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
	// e436eb7e-524f-11ce-9f53-0020af0ba770            MEDIASUBTYPE_RGB32 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_RGB32, 
		0xe436eb7e, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
	// 30323449-0000-0010-8000-00AA00389B71  'I420' ==  MEDIASUBTYPE_I420 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_I420, 
		0x30323449, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 56555949-0000-0010-8000-00AA00389B71  'IYUV' ==  MEDIASUBTYPE_IYUV 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_IYUV, 
		0x56555949, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 32315659-0000-0010-8000-00AA00389B71  'YV12' ==  MEDIASUBTYPE_YV12 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_YV12, 
		0x32315659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 32595559-0000-0010-8000-00AA00389B71  'YUY2' == MEDIASUBTYPE_YUY2 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_YUY2, 
		0x32595559, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 32323450-0000-0010-8000-00AA00389B71  'P422' == MEDIASUBTYPE_P422 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_P422, 
		0x32323450, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 59565955-0000-0010-8000-00AA00389B71  'UYVY' ==  MEDIASUBTYPE_UYVY 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_UYVY, 
		0x59565955, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 55595659-0000-0010-8000-00AA00389B71  'YVYU' == MEDIASUBTYPE_YVYU 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_YVYU, 
		0x55595659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 39555659-0000-0010-8000-00AA00389B71  'YVU9' == MEDIASUBTYPE_YVU9 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_YVU9, 
		0x39555659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 1d4a45f2-e5f6-4b44-8388-f0ae5c0e0c37            MEDIASUBTYPE_VIDEOIMAGE 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_VIDEOIMAGE,  
		0x1d4a45f2, 0xe5f6, 0x4b44, 0x83, 0x88, 0xf0, 0xae, 0x5c, 0x0e, 0x0c, 0x37)
	// 3334504D-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_MP43 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_MP43, 
		0x3334504D, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 5334504D-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_MP4S 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_MP4S, 
		0x5334504D, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 3253344D-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_M4S2 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_M4S2, 
		0x3253344D, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 31564D57-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMV1 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMV1, 
		0x31564D57, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 32564D57-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMV2 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMV2, 
		0x32564D57, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 3153534D-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_MSS1 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_MSS1, 
		0x3153534D, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// E06D8026-DB46-11CF-B4D1-00805F6CBBEA            WMMEDIASUBTYPE_MPEG2_VIDEO 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_MPEG2_VIDEO, 
		0xe06d8026, 0xdb46, 0x11cf, 0xb4, 0xd1, 0x00, 0x80, 0x5f, 0x6c, 0xbb, 0xea)
	// 73647561-0000-0010-8000-00AA00389B71  'auds' == WMMEDIATYPE_Audio 
	OUR_GUID_ENTRY(WMMEDIATYPE_Audio, 
		0x73647561, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 00000001-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_PCM 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_PCM, 
		0x00000001, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 00000009-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_DRM 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_DRM, 
		0x00000009, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 00000162-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMAudioV9 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMAudioV9, 
		0x00000162, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 00000163-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMAudio_Lossless 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMAudio_Lossless, 
		0x00000163, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 3253534D-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_MSS2 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_MSS2, 
		0x3253534D, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 0000000A-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMSP1 
	OUR_GUID_ENTRY( WMMEDIASUBTYPE_WMSP1, 
		0x0000000A,0x0000,0x0010,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71)
	// 0000000B-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMSP2 
	OUR_GUID_ENTRY( WMMEDIASUBTYPE_WMSP2, 
		0x0000000B,0x0000,0x0010,0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71)
	// 33564D57-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMV3 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMV3, 
		0x33564D57, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 50564D57-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMVP 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMVP, 
		0x50564D57, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 32505657-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WVP2 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WVP2, 
		0x32505657, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 41564D57-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMVA 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMVA, 
		0x41564D57, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 31435657-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WVC1 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WVC1, 
		0x31435657, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 00000161-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMAudioV8 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMAudioV8, 
		0x00000161, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 00000161-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMAudioV7 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMAudioV7, 
		0x00000161, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 00000161-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_WMAudioV2 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WMAudioV2, 
		0x00000161, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 00000130-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_ACELPnet 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_ACELPnet, 
		0x00000130, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 00000055-0000-0010-8000-00AA00389B71            WMMEDIASUBTYPE_MP3 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_MP3, 
		0x00000055, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71)
	// 776257D4-C627-41CB-8F81-7AC7FF1C40CC            WMMEDIASUBTYPE_WebStream 
	OUR_GUID_ENTRY(WMMEDIASUBTYPE_WebStream, 
		0x776257d4, 0xc627, 0x41cb, 0x8f, 0x81, 0x7a, 0xc7, 0xff, 0x1c, 0x40, 0xcc)
	// 73636d64-0000-0010-8000-00AA00389B71  'scmd' == WMMEDIATYPE_Script 
	OUR_GUID_ENTRY(WMMEDIATYPE_Script, 
		0x73636d64, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
	// 34A50FD8-8AA5-4386-81FE-A0EFE0488E31  'imag' == WMMEDIATYPE_Image 
	OUR_GUID_ENTRY(WMMEDIATYPE_Image, 
		0x34a50fd8, 0x8aa5, 0x4386, 0x81, 0xfe, 0xa0, 0xef, 0xe0, 0x48, 0x8e, 0x31)
	// D9E47579-930E-4427-ADFC-AD80F290E470  'fxfr' == WMMEDIATYPE_FileTransfer 
	OUR_GUID_ENTRY(WMMEDIATYPE_FileTransfer, 
		0xd9e47579, 0x930e, 0x4427, 0xad, 0xfc, 0xad, 0x80, 0xf2, 0x90, 0xe4, 0x70)
	// 9BBA1EA7-5AB2-4829-BA57-0940209BCF3E      'text' == WMMEDIATYPE_Text 
	OUR_GUID_ENTRY(WMMEDIATYPE_Text, 
		0x9bba1ea7, 0x5ab2, 0x4829, 0xba, 0x57, 0x9, 0x40, 0x20, 0x9b, 0xcf, 0x3e)
	// 05589F80-C356-11CE-BF01-00AA0055595A        WMFORMAT_VideoInfo 
	OUR_GUID_ENTRY(WMFORMAT_VideoInfo, 
		0x05589f80, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
	// E06D80E3-DB46-11CF-B4D1-00805F6CBBEA        WMFORMAT_MPEG2Video 
	OUR_GUID_ENTRY(WMFORMAT_MPEG2Video, 
		0xe06d80e3, 0xdb46, 0x11cf, 0xb4, 0xd1, 0x00, 0x80, 0x05f, 0x6c, 0xbb, 0xea)
	// 05589F81-C356-11CE-BF01-00AA0055595A        WMFORMAT_WaveFormatEx 
	OUR_GUID_ENTRY(WMFORMAT_WaveFormatEx, 
		0x05589f81, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
	// 5C8510F2-DEBE-4CA7-BBA5-F07A104F8DFF        WMFORMAT_Script 
	OUR_GUID_ENTRY(WMFORMAT_Script, 
		0x5c8510f2, 0xdebe, 0x4ca7, 0xbb, 0xa5, 0xf0, 0x7a, 0x10, 0x4f, 0x8d, 0xff)
	// DA1E6B13-8359-4050-B398-388E965BF00C        WMFORMAT_WebStream 
	OUR_GUID_ENTRY(WMFORMAT_WebStream, 
		0xda1e6b13, 0x8359, 0x4050, 0xb3, 0x98, 0x38, 0x8e, 0x96, 0x5b, 0xf0, 0x0c)
	// 82F38A70-C29F-11D1-97AD-00A0C95EA850        WMSCRIPTTYPE_TwoStrings 
	OUR_GUID_ENTRY( WMSCRIPTTYPE_TwoStrings, 
		0x82f38a70,0xc29f,0x11d1,0x97,0xad,0x00,0xa0,0xc9,0x5e,0xa8,0x50)

	OUR_GUID_ENTRY( WM_SampleExtensionGUID_OutputCleanPoint, 0xf72a3c6f, 0x6eb4, 0x4ebc, 0xb1, 0x92, 0x9, 0xad, 0x97, 0x59, 0xe8, 0x28 )
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_Timecode, 0x399595ec, 0x8667, 0x4e2d, 0x8f, 0xdb, 0x98, 0x81, 0x4c, 0xe7, 0x6c, 0x1e)
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_ChromaLocation, 0x4c5acca0, 0x9276, 0x4b2c, 0x9e, 0x4c, 0xa0, 0xed, 0xef, 0xdd, 0x21, 0x7e)
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_ColorSpaceInfo, 0xf79ada56, 0x30eb, 0x4f2b, 0x9f, 0x7a, 0xf2, 0x4b, 0x13, 0x9a, 0x11, 0x57 )
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_UserDataInfo, 0x732bb4fa, 0x78be, 0x4549, 0x99, 0xbd, 0x2, 0xdb, 0x1a, 0x55, 0xb7, 0xa8 )
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_FileName, 0xe165ec0e, 0x19ed, 0x45d7, 0xb4, 0xa7, 0x25, 0xcb, 0xd1, 0xe2, 0x8e, 0x9b)
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_ContentType, 0xd590dc20, 0x07bc, 0x436c, 0x9c, 0xf7, 0xf3, 0xbb, 0xfb, 0xf1, 0xa4, 0xdc )
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_PixelAspectRatio, 0x1b1ee554, 0xf9ea, 0x4bc8, 0x82, 0x1a, 0x37, 0x6b, 0x74, 0xe4, 0xc4, 0xb8 )
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_SampleDuration, 0xc6bd9450, 0x867f, 0x4907, 0x83, 0xa3, 0xc7, 0x79, 0x21, 0xb7, 0x33, 0xad )
	OUR_GUID_ENTRY( WM_SampleExtensionGUID_SampleProtectionSalt, 0x5403deee, 0xb9ee, 0x438f, 0xaa, 0x83, 0x38, 0x4, 0x99, 0x7e, 0x56, 0x9d )
};

int g_cOtherGuidNames = sizeof(g_OtherGuidNames) / sizeof(g_OtherGuidNames[0]);

#endif 


HRESULT CTextMediaType::AsText(FTL::CFStringFormater& strFormater)
{
	return ERROR_CALL_NOT_IMPLEMENTED;
}
//
// AsText
//
// Return the media type as a text string. Will place szAfterMajor after
// the text string for the major type and szAfterOthers after all other
// strings, apart from the last one.
//
HRESULT CTextMediaType::AsText(LPTSTR szType, unsigned int iLen)
{
    ASSERT(szType);
    HRESULT hr = E_FAIL;

    //
    // Convert Majortype to string
    //
	TCHAR szMajorType[100] = {0};
    UINT  iMajorType = 100;

    COM_VERIFY(CLSID2String(szMajorType, iMajorType, &majortype));
    szMajorType[99] = 0;        // Ensure null-termination

    //
    // Convert Subtype to string
    //
	TCHAR szSubType[100] = {0};
    UINT  iSubType = 100;
    COM_VERIFY(CLSID2String(szSubType, iSubType, &subtype));
    szSubType[99] = 0;        // Ensure null-termination

    //
    // Convert Format to string
    //
    TCHAR szFormat[500];
    COM_VERIFY(Format2String(szFormat, _countof(szFormat), FormatType(), Format(), FormatLength()));
    szFormat[_countof(szFormat) -1 ] = 0;        // Ensure null-termination

    COM_VERIFY(StringCchPrintf(szType, iLen, TEXT("Major:%s;  Sub:%s;  Format:%s"),
        szMajorType,szSubType, szFormat)); //GetSubtypeName(&subtype)

    return hr;
}


//
// CLSID2String
//
// Find a string description for a given GUID
//
HRESULT CTextMediaType::CLSID2String(LPTSTR szBuffer,
                                  UINT iLength,
                                  const GUID* pGuid)
{
    ASSERT(szBuffer);
    ASSERT(pGuid);
    HRESULT hr = S_OK;

    USES_CONVERSION;

    // Read the GUID name from the global table.  Use A2T macro
    // to convert to UNICODE string if necessary.

    TCHAR *pszGuidName = NULL;

#ifdef DETECT_OTHER_GUID_NAMES
    for (int i = 0; i < g_cOtherGuidNames; i++) {
        if (g_OtherGuidNames[i].guid == *pGuid) {
            pszGuidName =  A2T(g_OtherGuidNames[i].szName);
        }
    }
#endif //DETECT_OTHER_GUID_NAMES

    if (NULL == pszGuidName)
    {
        //CBasePin::DisplayTypeInfo 中为什么是 %hs 
        pszGuidName = A2T(GuidNames[*pGuid]);
    }

	TCHAR szGuidString[160] = {0};
	if (_tcscmp(pszGuidName, TEXT("Unknown GUID Name")) == 0)
	{
		StringFromGUID2(*pGuid, szGuidString, _countof(szGuidString));
		FTLTRACE(TEXT("CTextMediaType Unknown GUID %s\n"), szGuidString);
		pszGuidName = szGuidString;
		hr = S_FALSE;
	}
    size_t nLength = _tcslen(pszGuidName);

    _tcsncpy(szBuffer, pszGuidName, min(nLength, iLength));
    szBuffer[nLength] = 0;      // Ensure null-termination

    return hr;
}


//
// Format2String
//
// Converts a format block to a string
//
HRESULT CTextMediaType::Format2String(LPTSTR szBuffer,
                                   UINT iLength,
                                   const GUID* pFormatType,
                                   BYTE* pFormat,
                                   ULONG lFormatLength)
{
    UNREFERENCED_PARAMETER(lFormatLength);
    ASSERT(szBuffer);
    ASSERT(pFormatType);
    HRESULT hr = E_FAIL;

    //
    // Get the name of the format
    //
	TCHAR szName[50] = {0};
    UINT iName = 50;
    COM_VERIFY(CLSID2String(szName, iName, pFormatType));
    szName[49] = 0;     // Ensure null-termination

    //
    // Video Format
    //
    if (IsEqualGUID(*pFormatType, FORMAT_VideoInfo) ||
        IsEqualGUID(*pFormatType, FORMAT_MPEGVideo)) 
    {
		return _GetVideoInfoHeaderInfo(szBuffer, iLength, szName, pFormat, lFormatLength);
    }

	//
    // Audio Format
    //
    if (IsEqualGUID(*pFormatType, FORMAT_WaveFormatEx)) 
    {
		return _GetWaveFormatInfo(szBuffer, iLength, szName, pFormat, lFormatLength);
    }

    COM_VERIFY(StringCchPrintf(szBuffer, iLength, TEXT("%s"), szName));
    return hr;
}

HRESULT CTextMediaType::_GetVideoInfoHeaderInfo(LPTSTR szBuffer, 
	UINT iLength, LPCTSTR pszInfo, BYTE* pFormat, ULONG lFormatLength)
{
	HRESULT hr = S_OK;
	VIDEOINFOHEADER * pVideoFormat = (VIDEOINFOHEADER *) pFormat;

	if (pVideoFormat)
	{
		COM_VERIFY(StringCchPrintf(szBuffer, iLength, TEXT("%s %dx%d, %d bits")
			, (pVideoFormat->bmiHeader.biCompression == 0) ? TEXT("RGB") :
			((pVideoFormat->bmiHeader.biCompression == BI_BITFIELDS) ? TEXT("BITF") :
			(LPTSTR) &pVideoFormat->bmiHeader.biCompression )
			, pVideoFormat->bmiHeader.biWidth
			, pVideoFormat->bmiHeader.biHeight
			, pVideoFormat->bmiHeader.biBitCount));
	}
	return hr;
}

HRESULT CTextMediaType::_GetWaveFormatInfo(LPTSTR szBuffer, UINT iLength, 
						   LPCTSTR pszInfo, BYTE* pFormat, ULONG lFormatLength)
{
	HRESULT hr = S_OK;

	WAVEFORMATEX *pWaveFormat = (WAVEFORMATEX *) pFormat;
	if (pWaveFormat)
	{
		// !!! use ACM to get format type name?
		COM_VERIFY(StringCchPrintf(szBuffer, iLength
			, TEXT("%s: Tag(0x%04x,%s), nChannel(%d), %.3f KHz, %.3f kbps, ")
			  TEXT("BlockAlign(%d), BitsPerSample(%d), cbSize(%d)")
			, pszInfo
			, pWaveFormat->wFormatTag, FTL::CFDirectShowUtility::GetWaveFormatExTagString(pWaveFormat->wFormatTag)
			, pWaveFormat->nChannels
			, (double) pWaveFormat->nSamplesPerSec / 1000.0
			, (double)(pWaveFormat->nAvgBytesPerSec * 8) / 1000.0
			, pWaveFormat->nBlockAlign
			, pWaveFormat->wBitsPerSample
			, pWaveFormat->cbSize
			));
	}
	return hr;

}
