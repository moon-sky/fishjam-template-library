// Created by Microsoft (R) C/C++ Compiler Version 12.00.9782.0 (766ea23e).
//
// c:\documents and settings\administrator\×ÀÃæ\docfileview\debug\HexEditAx.tlh
//
// C++ source equivalent of Win32 type library E:\\Projects\\SuperPad\\HexEditAx\\Debug\\HexEditAx.dll
// compiler-generated file created 12/01/07 at 08:18:22 - DO NOT EDIT!

#pragma once
#pragma pack(push, 8)

#include <comdef.h>

namespace HexEditor {

//
// Forward references and typedefs
//

struct __declspec(uuid("c56a4603-9497-415e-b8b5-a2cf8e9e7525"))
/* dispinterface */ _IHexEditEvents;
struct /* coclass */ HexEdit;
struct __declspec(uuid("a206ca29-5543-448e-8862-ef4ca0d119e4"))
/* dual interface */ IHexEdit;

//
// Smart pointer typedef declarations
//

_COM_SMARTPTR_TYPEDEF(_IHexEditEvents, __uuidof(IDispatch));
_COM_SMARTPTR_TYPEDEF(IHexEdit, __uuidof(IHexEdit));

//
// Type library items
//

struct __declspec(uuid("c56a4603-9497-415e-b8b5-a2cf8e9e7525"))
_IHexEditEvents : IDispatch
{};

struct __declspec(uuid("1afd255e-3f12-472e-99fd-b1d89cb450d6"))
HexEdit;
    // [ default ] interface IHexEdit
    // [ default, source ] dispinterface _IHexEditEvents

struct __declspec(uuid("a206ca29-5543-448e-8862-ef4ca0d119e4"))
IHexEdit : IDispatch
{
    //
    // Raw methods provided by interface
    //

    virtual HRESULT __stdcall put_AutoSize (
        VARIANT_BOOL pbool ) = 0;
    virtual HRESULT __stdcall get_AutoSize (
        VARIANT_BOOL * pbool ) = 0;
    virtual HRESULT __stdcall put_BackColor (
        OLE_COLOR pclr ) = 0;
    virtual HRESULT __stdcall get_BackColor (
        OLE_COLOR * pclr ) = 0;
    virtual HRESULT __stdcall put_DrawMode (
        long pmode ) = 0;
    virtual HRESULT __stdcall get_DrawMode (
        long * pmode ) = 0;
    virtual HRESULT __stdcall putref_Font (
        IFontDisp * ppFont ) = 0;
    virtual HRESULT __stdcall put_Font (
        IFontDisp * ppFont ) = 0;
    virtual HRESULT __stdcall get_Font (
        IFontDisp * * ppFont ) = 0;
    virtual HRESULT __stdcall put_ForeColor (
        OLE_COLOR pclr ) = 0;
    virtual HRESULT __stdcall get_ForeColor (
        OLE_COLOR * pclr ) = 0;
    virtual HRESULT __stdcall put_Enabled (
        VARIANT_BOOL pbool ) = 0;
    virtual HRESULT __stdcall get_Enabled (
        VARIANT_BOOL * pbool ) = 0;
    virtual HRESULT __stdcall get_Data (
        VARIANT * pVal ) = 0;
    virtual HRESULT __stdcall put_Data (
        VARIANT pVal ) = 0;
    virtual HRESULT __stdcall get_ReadOnly (
        long * pVal ) = 0;
    virtual HRESULT __stdcall put_ReadOnly (
        long pVal ) = 0;
};

} // namespace HexEditor

#pragma pack(pop)
