

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Wed Feb 04 20:53:42 2015
 */
/* Compiler settings for .\MotleyFool.idl:
    Oicf, W1, Zp8, env=Win64 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif // __RPCPROXY_H_VERSION__


#include "MotleyFool.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _MotleyFool_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MotleyFool_MIDL_TYPE_FORMAT_STRING;

typedef struct _MotleyFool_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MotleyFool_MIDL_PROC_FORMAT_STRING;

typedef struct _MotleyFool_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } MotleyFool_MIDL_EXPR_FORMAT_STRING;


static RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const MotleyFool_MIDL_TYPE_FORMAT_STRING MotleyFool__MIDL_TypeFormatString;
extern const MotleyFool_MIDL_PROC_FORMAT_STRING MotleyFool__MIDL_ProcFormatString;
extern const MotleyFool_MIDL_EXPR_FORMAT_STRING MotleyFool__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IStockBar_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IStockBar_ProxyInfo;



#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const MotleyFool_MIDL_PROC_FORMAT_STRING MotleyFool__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const MotleyFool_MIDL_TYPE_FORMAT_STRING MotleyFool__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IStockBar, ver. 0.0,
   GUID={0x6DEEE498,0x08CC,0x43F0,{0xBC,0xA0,0xDB,0xB5,0xA2,0x5C,0x95,0x01}} */

#pragma code_seg(".orpc")
static const unsigned short IStockBar_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };

static const MIDL_STUBLESS_PROXY_INFO IStockBar_ProxyInfo =
    {
    &Object_StubDesc,
    MotleyFool__MIDL_ProcFormatString.Format,
    &IStockBar_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IStockBar_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    MotleyFool__MIDL_ProcFormatString.Format,
    &IStockBar_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(7) _IStockBarProxyVtbl = 
{
    0,
    &IID_IStockBar,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfoCount */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetTypeInfo */ ,
    0 /* (void *) (INT_PTR) -1 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION IStockBar_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _IStockBarStubVtbl =
{
    &IID_IStockBar,
    &IStockBar_ServerInfo,
    7,
    &IStockBar_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    MotleyFool__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x70001f4, /* MIDL Version 7.0.500 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * _MotleyFool_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IStockBarProxyVtbl,
    0
};

const CInterfaceStubVtbl * _MotleyFool_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IStockBarStubVtbl,
    0
};

PCInterfaceName const _MotleyFool_InterfaceNamesList[] = 
{
    "IStockBar",
    0
};

const IID *  _MotleyFool_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _MotleyFool_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _MotleyFool, pIID, n)

int __stdcall _MotleyFool_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_MotleyFool_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo MotleyFool_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _MotleyFool_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _MotleyFool_StubVtblList,
    (const PCInterfaceName * ) & _MotleyFool_InterfaceNamesList,
    (const IID ** ) & _MotleyFool_BaseIIDList,
    & _MotleyFool_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

