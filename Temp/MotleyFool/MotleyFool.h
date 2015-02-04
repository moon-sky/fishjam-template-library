

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __MotleyFool_h__
#define __MotleyFool_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IStockBar_FWD_DEFINED__
#define __IStockBar_FWD_DEFINED__
typedef interface IStockBar IStockBar;
#endif 	/* __IStockBar_FWD_DEFINED__ */


#ifndef __StockBar_FWD_DEFINED__
#define __StockBar_FWD_DEFINED__

#ifdef __cplusplus
typedef class StockBar StockBar;
#else
typedef struct StockBar StockBar;
#endif /* __cplusplus */

#endif 	/* __StockBar_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IStockBar_INTERFACE_DEFINED__
#define __IStockBar_INTERFACE_DEFINED__

/* interface IStockBar */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IStockBar;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6DEEE498-08CC-43F0-BCA0-DBB5A25C9501")
    IStockBar : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IStockBarVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStockBar * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStockBar * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStockBar * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStockBar * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStockBar * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStockBar * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStockBar * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IStockBarVtbl;

    interface IStockBar
    {
        CONST_VTBL struct IStockBarVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStockBar_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IStockBar_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IStockBar_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IStockBar_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IStockBar_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IStockBar_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IStockBar_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IStockBar_INTERFACE_DEFINED__ */



#ifndef __MOTLEYFOOLLib_LIBRARY_DEFINED__
#define __MOTLEYFOOLLib_LIBRARY_DEFINED__

/* library MOTLEYFOOLLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MOTLEYFOOLLib;

EXTERN_C const CLSID CLSID_StockBar;

#ifdef __cplusplus

class DECLSPEC_UUID("A6790AA5-C6C7-4BCF-A46D-0FDAC4EA90EB")
StockBar;
#endif
#endif /* __MOTLEYFOOLLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


