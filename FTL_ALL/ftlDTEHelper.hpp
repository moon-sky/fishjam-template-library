#ifndef FTL_DTE_HELPER_HPP
#define FTL_DTE_HELPER_HPP
#pragma once


#ifdef USE_EXPORT
#  include "ftlDTEHelper.h"
#endif
#include "ftlComDetect.h"

namespace FTL
{
    LPCTSTR CFDTEUtil::GetMsoControlTypeString(MsoControlType controlType)
    {
        switch(controlType)
        {
            HANDLE_CASE_RETURN_STRING(msoControlCustom);
            HANDLE_CASE_RETURN_STRING(msoControlButton);
            HANDLE_CASE_RETURN_STRING(msoControlEdit);
            HANDLE_CASE_RETURN_STRING(msoControlDropdown);
            HANDLE_CASE_RETURN_STRING(msoControlComboBox);
            HANDLE_CASE_RETURN_STRING(msoControlButtonDropdown);
            HANDLE_CASE_RETURN_STRING(msoControlSplitDropdown);
            HANDLE_CASE_RETURN_STRING(msoControlOCXDropdown);
            HANDLE_CASE_RETURN_STRING(msoControlGenericDropdown);
            HANDLE_CASE_RETURN_STRING(msoControlGraphicDropdown);
            HANDLE_CASE_RETURN_STRING(msoControlPopup);
            HANDLE_CASE_RETURN_STRING(msoControlGraphicPopup);
            HANDLE_CASE_RETURN_STRING(msoControlButtonPopup);
            HANDLE_CASE_RETURN_STRING(msoControlSplitButtonPopup);
            HANDLE_CASE_RETURN_STRING(msoControlSplitButtonMRUPopup);
            HANDLE_CASE_RETURN_STRING(msoControlLabel);
            HANDLE_CASE_RETURN_STRING(msoControlExpandingGrid);
            HANDLE_CASE_RETURN_STRING(msoControlSplitExpandingGrid);
            HANDLE_CASE_RETURN_STRING(msoControlGrid);
            HANDLE_CASE_RETURN_STRING(msoControlGauge);
            HANDLE_CASE_RETURN_STRING(msoControlGraphicCombo);
            HANDLE_CASE_RETURN_STRING(msoControlPane);
            HANDLE_CASE_RETURN_STRING(msoControlActiveX);
            HANDLE_CASE_RETURN_STRING(msoControlSpinner);
            HANDLE_CASE_RETURN_STRING(msoControlLabelEx);
            HANDLE_CASE_RETURN_STRING(msoControlWorkPane);
            HANDLE_CASE_RETURN_STRING(msoControlAutoCompleteCombo);
        default:
            FTLASSERT(FALSE);
            break;
        }
        return TEXT("Unknown");
    }

    LPCTSTR CFDTEUtil::GetMsoBarTypeString(MsoBarType barType)
    {
        switch(barType)
        {
            HANDLE_CASE_RETURN_STRING(msoBarTypeNormal);
            HANDLE_CASE_RETURN_STRING(msoBarTypeMenuBar);
            HANDLE_CASE_RETURN_STRING(msoBarTypePopup);
        default:
            FTLASSERT(FALSE);
            break;
        }
        return TEXT("Unknown");
    }

    LPCTSTR CFDTEUtil::GetWindowTypeString(EnvDTE::vsWindowType nWindowType)
    {
		using namespace EnvDTE;
        switch(nWindowType)
        {
            HANDLE_CASE_RETURN_STRING(vsWindowTypeCodeWindow);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeDesigner);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeBrowser);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeWatch);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeLocals);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeImmediate);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeSolutionExplorer);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeProperties);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeFind);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeFindReplace);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeToolbox);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeLinkedWindowFrame);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeMainWindow);
            HANDLE_CASE_RETURN_STRING(vsWindowTypePreview);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeColorPalette);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeToolWindow);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeDocument);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeOutput);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeTaskList);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeAutos);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeCallStack);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeThreads);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeDocumentOutline);
            HANDLE_CASE_RETURN_STRING(vsWindowTypeRunningDocuments);
        default:
            FTLASSERT(FALSE);
            break;
        }
        return TEXT("");
    }

    LPCTSTR CFDTEUtil::GetElementKindString(EnvDTE::vsCMElement nElementKind)
    {
		using namespace EnvDTE;
        switch(nElementKind)
        {
            HANDLE_CASE_RETURN_STRING(vsCMElementOther);
            HANDLE_CASE_RETURN_STRING(vsCMElementClass);
            HANDLE_CASE_RETURN_STRING(vsCMElementFunction);
            HANDLE_CASE_RETURN_STRING(vsCMElementVariable);
            HANDLE_CASE_RETURN_STRING(vsCMElementProperty);
            HANDLE_CASE_RETURN_STRING(vsCMElementNamespace);
            HANDLE_CASE_RETURN_STRING(vsCMElementParameter);
            HANDLE_CASE_RETURN_STRING(vsCMElementAttribute);
            HANDLE_CASE_RETURN_STRING(vsCMElementInterface);
            HANDLE_CASE_RETURN_STRING(vsCMElementDelegate);
            HANDLE_CASE_RETURN_STRING(vsCMElementEnum);
            HANDLE_CASE_RETURN_STRING(vsCMElementStruct);
            HANDLE_CASE_RETURN_STRING(vsCMElementUnion);
            HANDLE_CASE_RETURN_STRING(vsCMElementLocalDeclStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementFunctionInvokeStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementPropertySetStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementAssignmentStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementInheritsStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementImplementsStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementOptionStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementVBAttributeStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementVBAttributeGroup);
            HANDLE_CASE_RETURN_STRING(vsCMElementEventsDeclaration);
            HANDLE_CASE_RETURN_STRING(vsCMElementUDTDecl);
            HANDLE_CASE_RETURN_STRING(vsCMElementDeclareDecl);
            HANDLE_CASE_RETURN_STRING(vsCMElementDefineStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementTypeDef);
            HANDLE_CASE_RETURN_STRING(vsCMElementIncludeStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementUsingStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementMacro);
            HANDLE_CASE_RETURN_STRING(vsCMElementMap);
            HANDLE_CASE_RETURN_STRING(vsCMElementIDLImport);
            HANDLE_CASE_RETURN_STRING(vsCMElementIDLImportLib);
            HANDLE_CASE_RETURN_STRING(vsCMElementIDLCoClass);
            HANDLE_CASE_RETURN_STRING(vsCMElementIDLLibrary);
            HANDLE_CASE_RETURN_STRING(vsCMElementImportStmt);
            HANDLE_CASE_RETURN_STRING(vsCMElementMapEntry);
            HANDLE_CASE_RETURN_STRING(vsCMElementVCBase);
            HANDLE_CASE_RETURN_STRING(vsCMElementEvent);
            HANDLE_CASE_RETURN_STRING(vsCMElementModule);
        default:
            FTLASSERT(FALSE);
            break;
        }
        return TEXT("");
    }

    LPCTSTR CFDTEUtil::GetFunctionKindString(FTL::CFStringFormater& strFormater, int nFunctionKind)
    {
		using namespace EnvDTE;
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionConstructor, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPropertyGet, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPropertyLet, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPropertySet, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPutRef, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPropertyAssign, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionSub, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionFunction, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionTopLevel, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionDestructor, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionOperator, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionVirtual, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPure, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionConstant, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionShared, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionInline, TEXT(","));
        HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionComMethod, TEXT(","));
       
        FTLASSERT(0 == nFunctionKind);
        return strFormater.GetString();
    }

#define HANDLE_STRING_COMPARE_RETURN(val, exp, fun) \
    if(0 == fun((val),(exp))){\
        return TEXT(#exp);\
    }

    LPCTSTR CFDTEUtil::GetDTEGuidStringInfo(const CComBSTR& bstring)
    {
		using namespace EnvDTE;
        USES_CONVERSION;
        LPSTR pszCompareString = OLE2A(bstring);
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsDocumentKindText, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsDocumentKindHTML, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsDocumentKindResource, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsDocumentKindBinary, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsViewKindPrimary, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsViewKindAny, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsViewKindDebugging, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsViewKindCode, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsViewKindDesigner, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsViewKindTextView, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindTaskList, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindToolbox, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindCallStack, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindThread, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindLocals, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindAutoLocals, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindWatch, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindProperties, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindSolutionExplorer, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindOutput, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindObjectBrowser, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindObjectBrowser, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindMacroExplorer, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindDynamicHelp, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindClassView, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindResourceView, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindDocumentOutline, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindServerExplorer, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindCommandWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindFindSymbol, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindFindSymbolResults, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindFindReplace, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindFindResults1, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindFindResults2, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindMainWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindLinkedWindowFrame, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWindowKindWebBrowser, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWizardAddSubProject, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWizardAddItem, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsWizardNewProject, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectKindMisc, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectItemsKindMisc, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectItemKindMisc, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectKindUnmodeled, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectKindSolutionItems, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectItemsKindSolutionItems, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectItemKindSolutionItems, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectsKindSolution, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsAddInCmdGroup, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextSolutionBuilding, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextDebugging, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextFullScreenMode, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextDesignMode, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextNoSolution, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextEmptySolution, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextSolutionHasSingleProject, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextSolutionHasMultipleProjects, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextMacroRecording, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextMacroRecordingToolbar, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsMiscFilesProjectUniqueName, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsSolutionItemsProjectUniqueName, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectItemKindPhysicalFile, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectItemKindPhysicalFolder, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectItemKindVirtualFolder, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectItemKindSubProject, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_vk_Primary, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_vk_Debugging, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_vk_Code, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_vk_Designer, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_vk_TextView, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_TaskList, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_Toolbox, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_CallStackWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_ThreadWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_LocalsWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_AutoLocalsWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_WatchWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_ImmedWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_PropertyBrowser, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_SProjectWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_OutputWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_ObjectBrowser, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_ContextWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_ClassView, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_GUID_AddItemWizard, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_GUID_NewProjectWizard, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, dsCPP, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, dsHTML_IE3, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, dsHTML_RFC1866, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, dsFortran_Fixed, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, dsFortran_Free, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, dsJava, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, dsVBSMacro, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, dsIDL, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCATIDSolution, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCATIDSolutionBrowseObject, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCATIDMiscFilesProject, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCATIDMiscFilesProjectItem, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCATIDGenericProject, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCATIDDocument, strcmp );
        
        FTLTRACEEX(FTL::tlWarning, TEXT("Unknown Guid String: %s\n"), COLE2T(bstring));
        return TEXT("Unknown");
    }

    //////////////////////////////////////////////////////////////////////////

	//DTE提供了 ObjectExtenders、IVsProfferCommands、IVsIntelliMouseHandler 等 至少 60 多个Service
    HRESULT CFDTEDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("DTE")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::_DTE>     spDTE(m_pObj);
            if (spDTE)
            {
                CComBSTR    strName;
                COM_VERIFY(spDTE->get_Name(&strName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &strName));

                CComBSTR    strFileName;
                COM_VERIFY(spDTE->get_FileName(&strFileName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("FileName"), &strFileName));

                CComBSTR    strVersion;
                COM_VERIFY(spDTE->get_Version(&strVersion));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Version"), &strVersion));

				CComPtr<EnvDTE::Windows>    spWindows;
                COM_VERIFY(spDTE->get_Windows(&spWindows));
                CFWindowsDumper winsDumper(spWindows, pInfoOutput, m_nIndent + 2);

                CComPtr<EnvDTE::_Solution>  spSolution;
                COM_VERIFY(spDTE->get_Solution(&spSolution));
                CFSolutionDumper solutionDumper(spSolution, pInfoOutput, m_nIndent + 2);

                CComPtr<EnvDTE::Document>   spActiveDocument;
                COM_VERIFY(spDTE->get_ActiveDocument(&spActiveDocument));
                CFDocumentDumper    activeDocumentDumper(spActiveDocument, pInfoOutput, m_nIndent + 2);

                CComPtr<EnvDTE::Events> spEvents;
                COM_VERIFY(spDTE->get_Events(&spEvents));
                CFEventsDumper      eventsDumper(spEvents, pInfoOutput, m_nIndent + 2);
            }
        }
        return hr;
    }

    HRESULT CFSolutionDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Solution")));
        if (m_pObj)
        {
            CComQIPtr<EnvDTE::_Solution>     spSolution(m_pObj);
            if (spSolution)
            {
                CComBSTR bstrFileName;
                COM_VERIFY(spSolution->get_FileName(&bstrFileName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("FileName"), &bstrFileName));

                long nCount = 0;
                COM_VERIFY(spSolution->get_Count(&nCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("ProjectCount"), nCount));

                for (long nIndex = 1; nIndex <= nCount; nIndex++)
                {
                    CComPtr<EnvDTE::Project>    spProject;
                    COM_VERIFY(spSolution->Item(CComVariant(nIndex), &spProject));
                    CFProjectDumper projectDumper(spProject, pInfoOutput, m_nIndent + 2);
                }
            }
        }
        return hr;
    }

    HRESULT CFProjectDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Project")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::Project>     spProject(m_pObj);
            if (spProject)
            {
                CComBSTR bstrName;
                COM_VERIFY(spProject->get_Name(&bstrName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

                CComBSTR bstrKind;
                COM_VERIFY(spProject->get_Kind(&bstrKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kind"), CFDTEUtil::GetDTEGuidStringInfo(bstrKind)));

                CComPtr<EnvDTE::ProjectItems>   spProjectItems;
                COM_VERIFY(spProject->get_ProjectItems(&spProjectItems));
                CFProjectItemsDumper projectItemsDumper(spProjectItems, pInfoOutput, m_nIndent + 2);

                CComPtr<EnvDTE::CodeModel>  spCodeModel;
                COM_VERIFY(spProject->get_CodeModel(&spCodeModel));
                CFCodeModelDumper codeModelDumper(spCodeModel, pInfoOutput, m_nIndent + 2);
            }
        }
        return hr;
    }

    HRESULT CFProjectItemsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("ProjectItems")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::ProjectItems>     spProjectItems(m_pObj);
            if (spProjectItems)
            {
                CComBSTR bstrKind;
                COM_VERIFY(spProjectItems->get_Kind(&bstrKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kind"), &bstrKind));

                long nCount = 0;
                COM_VERIFY(spProjectItems->get_Count(&nCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Count"), nCount));

                for (long nIndex = 1; nIndex <= nCount; nIndex++)
                {
                    CComPtr<EnvDTE::ProjectItem>    spProjectItem;
                    COM_VERIFY(spProjectItems->Item(CComVariant(nIndex), &spProjectItem));
                    CFProjectItemDumper projectItemDumper(spProjectItem, pInfoOutput, m_nIndent + 2);
                }
            }
        }
        return hr;
    }

    HRESULT CFProjectItemDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("ProjectItem")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::ProjectItem>     spProjectItem(m_pObj);
            if (spProjectItem)
            {
                CComBSTR bstrName;
                COM_VERIFY(spProjectItem->get_Name(&bstrName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

                CComBSTR bstrKind;
                COM_VERIFY(spProjectItem->get_Kind(&bstrKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kind"), CFDTEUtil::GetDTEGuidStringInfo(bstrKind)));
    
                CComPtr<IDispatch> spProjectItemModel;
                COM_VERIFY(spProjectItem->get_Object(&spProjectItemModel));
                if (SUCCEEDED(hr) && spProjectItemModel)
                {
                    COM_DETECT_INTERFACE_FROM_LIST(spProjectItemModel);
                }
                
                CComPtr<EnvDTE::FileCodeModel> spFileCodeModel;
                COM_VERIFY_EXCEPT1(spProjectItem->get_FileCodeModel(&spFileCodeModel), S_FALSE);
                if (SUCCEEDED(hr) && spFileCodeModel)
                {
                    COM_DETECT_INTERFACE_FROM_LIST(spFileCodeModel);
                }
                CFFileCodeModelDumper fileCodeModelDumper(spFileCodeModel, pInfoOutput, m_nIndent + 2);

                CComVariant varExtenderNames;
                COM_VERIFY(spProjectItem->get_ExtenderNames(&varExtenderNames));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("ExtenderNames"), &varExtenderNames));

                short sFileCount = 0;
                COM_VERIFY(spProjectItem->get_FileCount(&sFileCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Count"), sFileCount));

                for (short index = 1; index <= sFileCount; index++)
                {
                    CComBSTR bstrFileName;
                    COM_VERIFY(spProjectItem->get_FileNames(index, &bstrFileName));
                    COM_VERIFY(pInfoOutput->OnOutput(TEXT("FileNames"),sFileCount, index, &CComVariant(bstrFileName)));
                }
            }
        }
        return hr;
    }

    HRESULT CFEventsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Events")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::Events>     spEvents(m_pObj);
            if (spEvents)
            {

            }
        }
        return hr;
    }

    HRESULT CFWindowsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Windows")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::Windows>     spWindows(m_pObj);
            if (spWindows)
            {
                long nCount = 0;
                COM_VERIFY(spWindows->get_Count(&nCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Count"), nCount));

                for (long index = 1; index <= nCount; index++)
                {
                    CComPtr<EnvDTE::Window> spWindow;
                    COM_VERIFY(spWindows->Item(CComVariant(index), &spWindow));
                    CFWindowDumper winDumper(spWindow, pInfoOutput, m_nIndent + 2);
                }

                CComPtr<EnvDTE::_DTE> spDTE;
                COM_VERIFY(spWindows->get_DTE(&spDTE));
                CComPtr<EnvDTE::_DTE> spDTEParent;
                COM_VERIFY(spWindows->get_Parent(&spDTEParent));
                if (spDTE != spDTEParent)
                {
					FTLTRACEEX(FTL::tlWarning, TEXT("DTE and DTEParent is Not Same\n"));
                }
            }
        }
        return hr;
    }

    HRESULT CFWindowDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Window")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::Window>     spWindow(m_pObj);
            if (spWindow)
            {
                CComBSTR bstrCaption;
                COM_VERIFY(spWindow->get_Caption(&bstrCaption));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Caption"), &bstrCaption));
                
                VARIANT_BOOL bVisible = VARIANT_TRUE;
                COM_VERIFY(spWindow->get_Visible(&bVisible));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Visible"), (long)bVisible));

                long nLeft = 0, nTop = 0, nWidth = 0, nHeight = 0;
                COM_VERIFY(spWindow->get_Left(&nLeft));
                COM_VERIFY(spWindow->get_Top(&nTop));
                COM_VERIFY(spWindow->get_Width(&nWidth));
                COM_VERIFY(spWindow->get_Height(&nHeight));

                CFStringFormater strPosition;
                strPosition.Format(TEXT("[%d,%d], {%d x %d}"), nLeft, nTop, nWidth, nHeight);
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("[left,top],{width x Height}"), strPosition.GetString()));

                EnvDTE::vsWindowType winType;
                COM_VERIFY(spWindow->get_Type(&winType));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Type"), CFDTEUtil::GetWindowTypeString(winType)));

                //DTE中除了MainWindow能获取到HWnd外，其他都不能获得，
                //要想获取窗体的HWND，需要通过 IVsTextView 等获取?
                long lHWnd = 0;
                COM_VERIFY(spWindow->get_HWnd(&lHWnd));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("HWwnd"), lHWnd));

                CComBSTR bstrKind;
                COM_VERIFY(spWindow->get_Kind(&bstrKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kind"), &bstrKind));

                CComBSTR bstrObjectKind;
                COM_VERIFY(spWindow->get_ObjectKind(&bstrObjectKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("ObjectKind"), CFDTEUtil::GetDTEGuidStringInfo(bstrObjectKind)));

                CComPtr<IDispatch> spToolObject;
                COM_VERIFY(spWindow->get_Object(&spToolObject));
                if (spToolObject)
                {
					//在代码编辑 Window 中，能够得到 TextWindow
                    COM_DETECT_INTERFACE_FROM_LIST(spToolObject);
                }
                //Document 窗口可以获取 Selection
                //spWindow->get_Selection();
            }
        }
        return hr;
    }


    //Document
    HRESULT CFDocumentDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Document")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::Document>     spDocument(m_pObj);
            if (spDocument)
            {
                CComBSTR bstrKind;
                COM_VERIFY(spDocument->get_Kind(&bstrKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kind"), CFDTEUtil::GetDTEGuidStringInfo(bstrKind)));

                CComBSTR bstrFullName;
                COM_VERIFY(spDocument->get_FullName(&bstrFullName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("FullName"), &bstrFullName));

                CComPtr<IDispatch> spSelection;
                COM_VERIFY(spDocument->get_Selection(&spSelection));
                COM_DETECT_INTERFACE_FROM_LIST(spSelection);
                CFTextSelectionDumper   selectionDumper(spSelection, pInfoOutput, m_nIndent + 2);

                CComPtr<EnvDTE::Window> spActiveWindow;
                COM_VERIFY(spDocument->get_ActiveWindow(&spActiveWindow));
                CFWindowDumper  activeWindowDumper(spActiveWindow, pInfoOutput, m_nIndent + 2);

                CComPtr<IDispatch> spTextDocumentDisp;
                COM_VERIFY(spDocument->Object(CComBSTR(L"TextDocument"),&spTextDocumentDisp));
                if (SUCCEEDED(hr) && spTextDocumentDisp)
                {
                    CComQIPtr<TextDocument> spTextDoc=spTextDocumentDisp;
                    CFTextDocumentDumper textDocumentDumper(spTextDoc, pInfoOutput, m_nIndent + 2);
                }

            }
        }
        return hr;
    }

    HRESULT CFTextDocumentDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("TextDocument")));
        if (m_pObj)
        {
            CComQIPtr<EnvDTE::TextDocument>     spTextDocument(m_pObj);
            if (spTextDocument)
            {TextSelection
                FTLASSERT(FALSE);
                /*
                //Get TextPoint
                CComPtr<TextPoint> StartTextPoint;
                hr=textDoc->get_StartPoint(&StartTextPoint);
                if ( FAILED(hr) )    return false;
                //get EditPoint
                //    CComPtr<EditPoint> StratEditPoint;
                hr=StartTextPoint->CreateEditPoint(&StratEditPoint);
                if ( FAILED(hr) )    return false;
                */
            }
        }
        return hr;
    }

    HRESULT CFTextSelectionDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("TextSelection")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::TextSelection> spTextSelection(m_pObj);
            if (spTextSelection)
            {

            }
        }
        return hr;
    }

    //Code

    HRESULT CFFileCodeModelDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("FileCodeModel")));
        if (m_pObj)
        {
            CComQIPtr<EnvDTE::FileCodeModel>    spFileCodeModel(m_pObj);
            if (spFileCodeModel)
            {
                CComBSTR bstrLanguage;
                COM_VERIFY(spFileCodeModel->get_Language(&bstrLanguage));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Language"), &bstrLanguage));

                CComPtr<EnvDTE::CodeElements>   spCodeElements;
                COM_VERIFY(spFileCodeModel->get_CodeElements(&spCodeElements));
                CFCodeElementsDumper   codeElementsDumper(spCodeElements, pInfoOutput, m_nIndent + 2);
            }
        }
        return hr;
    }

    HRESULT CFCodeModelDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("CodeModel")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::CodeModel>     spCodeModel(m_pObj);
            if (spCodeModel)
            {
                CComBSTR bstrLanguage;
                COM_VERIFY(spCodeModel->get_Language(&bstrLanguage));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Language"), CFDTEUtil::GetDTEGuidStringInfo(bstrLanguage)));

                CComPtr<EnvDTE::CodeElements>   spCodeElements;
                COM_VERIFY(spCodeModel->get_CodeElements(&spCodeElements));
                CFCodeElementsDumper   codeElementsDumper(spCodeElements, pInfoOutput, m_nIndent + 2);

            }
        }
        return hr;
    }

    HRESULT CFCodeElementsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("CodeElements")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::CodeElements>     spCodeElements(m_pObj);
            if (spCodeElements)
            {
                long nCount = 0;
                COM_VERIFY(spCodeElements->get_Count(&nCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Count"), nCount));

                for (long nIndex = 1; nIndex <= nCount; nIndex++)
                {
                    CComPtr<EnvDTE::CodeElement> spCodeElement;
                    COM_VERIFY(spCodeElements->Item(CComVariant(nIndex), &spCodeElement));
                    CFCodeElementDumper codeElementDumper(spCodeElement, pInfoOutput, m_nIndent + 2);
                }
            }
        }
        return hr;
    }

    HRESULT CFCodeElementDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("CodeElement")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::CodeElement>     spCodeElement(m_pObj);
            if (spCodeElement)
            {
                CComBSTR bstrName;
                COM_VERIFY(spCodeElement->get_Name(&bstrName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

                CComBSTR bstrFullName;
                COM_VERIFY(spCodeElement->get_FullName(&bstrFullName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("FullName"), &bstrFullName));

                EnvDTE::vsCMElement elementKind = EnvDTE::vsCMElementOther;
                COM_VERIFY(spCodeElement->get_Kind(&elementKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("FullName"), CFDTEUtil::GetElementKindString(elementKind)));
            }
        }
        return hr;
    }

    HRESULT CFCommandBarsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("_CommandBars")));
        if (m_pObj)
        {
			CComQIPtr<Microsoft_VisualStudio_CommandBars::_CommandBars>     spCommandBars(m_pObj);
            if (spCommandBars)
            {
                int nToolBarCount = 0;
                COM_VERIFY(spCommandBars->get_Count(&nToolBarCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Count"), nToolBarCount));
                for (int nToolBarIndex = 1; nToolBarIndex <= nToolBarCount; ++ nToolBarIndex)
                {
                    CComPtr<Microsoft_VisualStudio_CommandBars::CommandBar>  spCommandBar;
                    COM_VERIFY(spCommandBars->get_Item(CComVariant(nToolBarIndex), &spCommandBar));
                    CFCommandBarDumper commandBarDumper(spCommandBar, pInfoOutput, m_nIndent + 2);
                }

                CComPtr<Microsoft_VisualStudio_CommandBars::CommandBarControl>	spActiveControl;
                COM_VERIFY(spCommandBars->get_ActionControl(&spActiveControl));
                if (spActiveControl)
                {
                    CFCommandBarControlDumper commandBarControlDumper(spActiveControl, pInfoOutput, m_nIndent + 2);
                }
            }
        }
        return hr;
    }

    HRESULT CFCommandBarDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("CommandBar")));
        if (m_pObj)
        {
            CComQIPtr<Microsoft_VisualStudio_CommandBars::CommandBar>     spCommandBar(m_pObj);
            if (spCommandBar)
            {
                CComBSTR bstrName;
                COM_VERIFY(spCommandBar->get_Name(&bstrName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

                MsoBarType barType;
                COM_VERIFY(spCommandBar->get_Type(&barType));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Type"), CFDTEUtil::GetMsoBarTypeString(barType)));
                //VARIANT_BOOL	isBuiltIn = VARIANT_FALSE;
                //COM_VERIFY(spCommandBar->get_BuiltIn(&isBuiltIn));
                //COM_VERIFY(pInfoOutput->OnOutput(TEXT("BuiltIn"), isBuiltIn));

                CComBSTR bstrContext;
                COM_VERIFY(spCommandBar->get_Context(&bstrContext));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Context"), &bstrContext));

                int nIndex = 0;
                COM_VERIFY(spCommandBar->get_Index(&nIndex));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Index"), nIndex));

                int nLeft = 0, nTop = 0, nWidth = 0, nHeight = 0;
                
                COM_VERIFY(spCommandBar->get_Left(&nLeft));
                COM_VERIFY(spCommandBar->get_Top(&nTop));
                COM_VERIFY(spCommandBar->get_Width(&nWidth));
                COM_VERIFY(spCommandBar->get_Height(&nHeight));
                FTL::CFStringFormater formater;
                formater.Format(TEXT("[%d,%d], {%d x %d}"), nLeft, nTop, nWidth, nHeight);
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("[left,top], {width x height}"), formater.GetString()));

                CComPtr<CommandBarControls> spCommandBarControls;
                COM_VERIFY(spCommandBar->get_Controls(&spCommandBarControls));
                if (spCommandBarControls)
                {
                    CFCommandBarControlsDumper commandBarControlsDumper(spCommandBarControls, pInfoOutput, m_nIndent + 2);
                }
            }
        }
        return hr;
    }

    HRESULT CFCommandBarControlsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("CommandBarControls")));
        if (m_pObj)
        {
            CComQIPtr<CommandBarControls>     spCommandBarControls(m_pObj);
            if (spCommandBarControls)
            {
                int nCount = 0;
                COM_VERIFY(spCommandBarControls->get_Count(&nCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Count"), nCount));
                for (int nIndex = 1; nIndex <= nCount; ++nIndex)
                {
                    CComPtr<CommandBarControl>	spCommandBarControl;
                    COM_VERIFY(spCommandBarControls->get_Item(CComVariant(nIndex), &spCommandBarControl));
                    if (spCommandBarControl)
                    {
                        CFCommandBarControlDumper commandBarControlDumper(spCommandBarControl, pInfoOutput, m_nIndent + 2);
                    }
                }
            }
        }
        return hr;
    }

    HRESULT CFCommandBarControlDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("CommandBarControl")));
        if (m_pObj)
        {
            CComQIPtr<CommandBarControl>     spCommandBarControl(m_pObj);
            if (spCommandBarControl)
            {
                CComBSTR bstrCaptrion;
                COM_VERIFY(spCommandBarControl->get_Caption(&bstrCaptrion));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Caption"), &bstrCaptrion));

                MsoControlType controlType = msoControlCustom;
                COM_VERIFY(spCommandBarControl->get_Type(&controlType));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Type"), CFDTEUtil::GetMsoControlTypeString(controlType)));

#if 0
                //All is ""
                CComBSTR bstrDescriptionText;
                COM_VERIFY(spCommandBarControl->get_DescriptionText(&bstrDescriptionText));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("DescriptionText"), &bstrDescriptionText));
#endif 

                CComBSTR bstrTooltipText;
                COM_VERIFY(spCommandBarControl->get_TooltipText(&bstrTooltipText));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("TooltipText"), &bstrTooltipText));

                int nId = -1;
                COM_VERIFY(spCommandBarControl->get_Id(&nId));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Id"), nId));

                int nIndex = -1;
                COM_VERIFY(spCommandBarControl->get_Index(&nIndex));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Index"), nIndex));

                long lInstanceId = -1;
                COM_VERIFY(spCommandBarControl->get_InstanceId(&lInstanceId));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("InstanceId"), lInstanceId));

                VARIANT_BOOL bVisible = VARIANT_FALSE;
                COM_VERIFY(spCommandBarControl->get_Visible(&bVisible));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Visible"), bVisible));

                CComBSTR bstrTag;
                COM_VERIFY(spCommandBarControl->get_Tag(&bstrTag));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Tag"), &bstrTag));

                CComBSTR bstrOnAction;
                COM_VERIFY_EXCEPT1(spCommandBarControl->get_OnAction(&bstrOnAction), DISP_E_MEMBERNOTFOUND);
                if (SUCCEEDED(hr) && bstrOnAction)
                {
                    COM_VERIFY(pInfoOutput->OnOutput(TEXT("OnAction"), &bstrOnAction));
                }
            }
        }
        return hr;
    }


    HRESULT CFCommandsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Commands")));

        if (m_pObj)
        {
            CComQIPtr<EnvDTE::Commands>     spCommands(m_pObj);
            if (spCommands)
            {
                long lCommandCount = 0;
                COM_VERIFY(spCommands->get_Count(&lCommandCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Count"), lCommandCount));

                for (long lCommandIndex = 1; lCommandIndex <= lCommandCount; ++lCommandIndex)
                {
                    CComPtr<EnvDTE::Command>  spCommand;
                    //TODO: ( pCommands->Item( CComVariant( "DoxygenAddin.Connect.DoxygenAddin" ), 0, &dx_cmd ), dx_cmd );
                    //  Need Text as Index ?
                    //用目前这种方式也可以获取，但ID有什么用？
                    COM_VERIFY(spCommands->Item(CComVariant(lCommandIndex),0, &spCommand));
                    if (spCommand)
                    {
                        CFCommandDumper commandDumper(spCommand, pInfoOutput, m_nIndent + 2);
                    }
                }
            }
        }
        return hr;
    }

    HRESULT CFCommandDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Command")));
        if (m_pObj)
        {
            CComQIPtr<EnvDTE::Command>     spCommand(m_pObj);
            if (spCommand)
            {
                CComBSTR bstrName;
                COM_VERIFY(spCommand->get_Name(&bstrName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

                CComBSTR bstrLocalizedName;
                COM_VERIFY(spCommand->get_LocalizedName(&bstrLocalizedName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("LocalizedName"), &bstrLocalizedName));

                CComBSTR bstrGuid;
                //Guid 有什么用？很多是一样，比如 File.FormProperties 和 Edit.ChangeBinding
                COM_VERIFY(spCommand->get_Guid(&bstrGuid));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Guid"), &bstrGuid));

                long nId = -1;
                COM_VERIFY(spCommand->get_ID(&nId));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("ID"), nId));

                CComVariant varBindings; //BSTR
                COM_VERIFY(spCommand->get_Bindings(&varBindings));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Bindings"), &varBindings));
            }
        }
        return hr;
    }

}


#endif //FTL_FILE_HPP