#ifndef FTL_DTE_HELPER_HPP
#define FTL_DTE_HELPER_HPP
#pragma once


#ifdef USE_EXPORT
#  include "ftlDTEHelper.h"
#endif
#include "ftlComDetect.h"

namespace FTL
{
#ifdef MSO
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
#endif 

    LPCTSTR CFDTEUtil::GetWindowTypeString(DTE_NS::vsWindowType nWindowType)
    {
		using namespace DTE_NS;
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

    LPCTSTR CFDTEUtil::GetElementKindString(DTE_NS::vsCMElement nElementKind)
    {
		using namespace DTE_NS;
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
		using namespace DTE_NS;
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

#ifdef VCCML_NS
	LPCTSTR CFDTEUtil::GetCMFunctionKindString(FTL::CFStringFormater& strFormater, int nFunctionKind)
	{
		using namespace VCCML_NS;
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionConstructor , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPropertyGet , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPropertyLet , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPropertySet , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPutRef , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPropertyAssign , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionSub , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionFunction , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionTopLevel , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionDestructor , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionOperator , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionVirtual , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionPure , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionConstant , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionShared , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionInline , TEXT(","));
		HANDLE_COMBINATION_VALUE_TO_STRING(strFormater, nFunctionKind, vsCMFunctionComMethod , TEXT(","));

		FTLASSERT(0 == nFunctionKind);
		return strFormater.GetString();
	}
#endif 

#ifdef FTL_DEBUG
#  define HANDLE_STRING_COMPARE_RETURN(val, exp, fun) \
    	if(0 == fun((val),(exp))){\
		    if(bFound)\
			{\
				FTLTRACEEX(tlWarning, TEXT("%s, Warning: Find Same String: %s <=> %s \n"), __FILE__LINE__,\
					pszResult, TEXT(#exp));\
				FTLASSERT(!bFound);\
			}\
			bFound = TRUE;\
			pszResult = TEXT(#exp);\
		}
#else
  #define HANDLE_STRING_COMPARE_RETURN(val, exp, fun) \
		if(0 == fun((val),(exp))){\
			return TEXT(#exp);\
		}
#endif //FTL_DEBUG

    LPCTSTR CFDTEUtil::GetDTEGuidStringInfo(const CComBSTR& bstring)
    {
#ifdef FTL_DEBUG
		LPCTSTR pszResult = NULL;
		BOOL bFound = FALSE;
#endif

		//Project Type Description -- http://thorpe.blog.sohu.com/142017141.html
		static const LPCSTR vsProjectTypeWindowsCSharp = "{FAE04EC0-301F-11D3-BF4B-00C04F79EFBC}"; //Windows (C#)
		static const LPCSTR vsProjectTypeWindowsVBNet = "{F184B08F-C81C-45F6-A57F-5ABD9991F28F}"; //Windows (VB.NET)
		static const LPCSTR vsProjectTypeWindowsVCPlusPlus = "{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}"; //Windows (Visual C++)
		static const LPCSTR vsProjectTypeWebApplication = "{349C5851-65DF-11DA-9384-00065B846F21}"; //Web Application
		static const LPCSTR vsProjectTypeWebSite = "{E24C65DC-7377-472B-9ABA-BC803B73C61A}"; //Web Site
		static const LPCSTR vsProjectTypeDistributedSystem = "{F135691A-BF7E-435D-8960-F99683D2D49C}"; //Distributed System
		static const LPCSTR vsProjectTypeWCF = "{3D9AD99F-2412-4246-B90B-4EAA41C64699}"; //Windows Communication Foundation (WCF)
		static const LPCSTR vsProjectTypeWPF = "{60DC8134-EBA5-43B8-BCC9-BB4BC16C2548}"; //Windows Presentation Foundation (WPF)
		static const LPCSTR vsProjectTypeVisualDatabaseTools = "{C252FEB5-A946-4202-B1D4-9916A0590387}"; //Visual Database Tools
		static const LPCSTR vsProjectTypeDatabase = "{A9ACE9BB-CECE-4E62-9AA4-C7E7C5BD2124}"; //Database
		static const LPCSTR vsProjectTypeDatabaseOther = "{4F174C21-8C12-11D0-8340-0000F80270F8}"; //Database (other project types)
		static const LPCSTR vsProjectTypeTest = "{3AC096D0-A1C2-E12C-1390-A8335801FDAB}"; //Test
		static const LPCSTR vsProjectTypeLeagacy2003SmartDeviceCSharp = "{20D4826A-C6FA-45DB-90F4-C717570B9F32}"; //Legacy (2003) Smart Device (C#)
		static const LPCSTR vsProjectTypeLeagacy2003SmartDeviceVBNet = "{CB4CE8C6-1BDB-4DC7-A4D3-65A1999772F8}"; //Legacy (2003) Smart Device (VB.NET)
		static const LPCSTR vsProjectTypeSmartDeviceCSharp = "{4D628B5B-2FBC-4AA6-8C16-197242AEB884}"; //Smart Device (C#)
		static const LPCSTR vsProjectTypeSmartDeviceVBNet = "{68B1623D-7FB9-47D8-8664-7ECEA3297D4F}"; //Smart Device (VB.NET)
		static const LPCSTR vsProjectTypeWorkflowCSharp = "{14822709-B5A1-4724-98CA-57A101D1B079}"; //Workflow (C#)
		static const LPCSTR vsProjectTypeWorkflowVBNet = "{D59BE175-2ED0-4C54-BE3D-CDAA9F3214C8}"; //Workflow (VB.NET)
		static const LPCSTR vsProjectTypeDeploymentMergeModule = "{06A35CCD-C46D-44D5-987B-CF40FF872267}"; //Deployment Merge Module
		static const LPCSTR vsProjectTypeDeploymentCab = "{3EA9E505-35AC-4774-B492-AD1749C4943A}"; //Deployment Cab
		static const LPCSTR vsProjectTypeDeploymentSetup = "{978C614F-708E-4E1A-B201-565925725DBA}"; //Deployment Setup
		static const LPCSTR vsProjectTypeDeploymentSmartDeviceCab = "{AB322303-2255-48EF-A496-5904EB18DA55}"; //Deployment Smart Device Cab
		static const LPCSTR vsProjectTypeVSTA = "{A860303F-1F3F-4691-B57E-529FC101A107}"; //Visual Studio Tools for Applications (VSTA)
		static const LPCSTR vsProjectTypeVSTO = "{BAA0C2D2-18E2-41B9-852F-F413020CAA33}"; //Visual Studio Tools for Office (VSTO)
		static const LPCSTR vsProjectTypeSharePointWorkflow = "{F8810EC1-6754-47FC-A15F-DFABD2E3FA90}"; //SharePoint Workflow
		static const LPCSTR vsProjectTypeXNAWindows = "{6D335F3A-9D43-41b4-9D22-F6F17C4BE596}"; //XNA (Windows)
		static const LPCSTR vsProjectTypeXNAXBox = "{2DF5C3F4-5A5F-47a9-8E94-23B4456F55E2}"; //XNA (XBox)
		static const LPCSTR vsProjectTypeXNAZune = "{D399B71A-8929-442a-A9AC-8BEC78BB2433}"; //XNA (Zune)
		static const LPCSTR vsProjectTypeSharePointVBNet = "{EC05E597-79D4-47f3-ADA0-324C4F7C7484}"; //SharePoint (VB.NET)
		static const LPCSTR vsProjectTypeSharePointCSharp = "{593B0543-81F6-4436-BA1E-4747859CAAE2}"; //SharePoint (C#)
		static const LPCSTR vsProjectTypeSilverlight = "{A1591282-1198-4647-A2B1-27E5FF5F6F3B}"; //Silverlight
		static const LPCSTR vsProjectTypeAspNetMVCApplication = "{603C0E0B-DB56-11DC-BE95-000D561079B0}"; //ASP.Net MVC Application

		static const LPCSTR vsConstVsTextView ="{F5E7E71E-1401-11d1-883B-0000F87579D2}"; //textmgr.h 中的 VsTextView, 对应IVsCodeWindow的ViewClassID

		//static const LPCSTR vsConstXXX ="{XXXX}"; //

		
		using namespace DTE_NS;
        USES_CONVERSION;
        LPSTR pszCompareString = OLE2A(bstring);

#pragma TODO(duplicate test)
		//dteinternal.h / dte80a.tlh
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
        //HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_Toolbox, strcmp );  //vsWindowKindToolbox
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_CallStackWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_ThreadWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_LocalsWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_AutoLocalsWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_WatchWindow, strcmp );
        HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_ImmedWindow, strcmp );
        //HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_PropertyBrowser, strcmp );	//vsWindowKindProperties
        //HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_SProjectWindow, strcmp );	//vsWindowKindSolutionExplorer
        //HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsext_wk_OutputWindow, strcmp );		//vsWindowKindOutput
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

		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCMLanguageVC, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCMLanguageVB, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCMLanguageCSharp, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCMLanguageIDL, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsCMLanguageMC, strcmp );
        

		//dte80.h(VSIP)
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidSolutionBuilding, strcmp );
		
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidUIHierarchyDragging, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidFullScreenMode, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidDesignMode, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidNoSolution, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidSolutionExists, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidEmptySolution, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidSolutionHasSingleProject, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidSolutionHasMultipleProjects, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidCodeWindow, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidNotBuildingAndNotDebugging, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidSolutionOrProjectUpgrading, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidDataSourceWindowSupported, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidDataSourceWindowAutoVisible, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidWindowsFormsDesigner, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidToolboxInitialized, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidSolutionExistsAndNotBuildingAndNotDebugging, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidTextEditor, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidXMLTextEditor, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidCSSTextEditor, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidHTMLSourceEditor, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidHTMLDesignView, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidHTMLSourceView, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidHTMLCodeView, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidFrames, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidSchema, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidData, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidKindStartPage, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidCommunityWindow, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidDeviceExplorer, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidBookmarks, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidApplicationBrowser, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidFavorites, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidErrorList, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidHelpSearch, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidHelpIndex, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidHelpContents, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidCallBrowser, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidCodeDefinition, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidTaskList, strcmp );
		//HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidToolbox, strcmp );			//vsWindowKindToolbox
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidCallStack, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidThread, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidLocals, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidAutoLocals, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidWatch, strcmp );
		//HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidProperties, strcmp );		//vsWindowKindProperties
		//HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidSolutionExplorer, strcmp );	//vsWindowKindSolutionExplorer
		//HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidOutput, strcmp );			//vsWindowKindOutput
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidObjectBrowser, strcmp );
		//HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidMacroExplorer, strcmp );		//vsWindowKindMacroExplorer
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidDynamicHelp, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidClassView, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidResourceView, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidDocumentOutline, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidServerExplorer, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidCommandWindow, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidFindSymbol, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidFindSymbolResults, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidFindReplace, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidFindResults1, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidFindResults2, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidMainWindow, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidLinkedWindowFrame, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsContextGuidWebBrowser, strcmp );

		//self defined
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWindowsCSharp, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWindowsVBNet, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWindowsVCPlusPlus, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWebApplication, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWebSite, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeDistributedSystem, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWCF, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWPF, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeVisualDatabaseTools, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeDatabase, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeDatabaseOther, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeTest, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeLeagacy2003SmartDeviceCSharp, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeLeagacy2003SmartDeviceVBNet, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeSmartDeviceCSharp, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeSmartDeviceVBNet, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWorkflowCSharp, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeWorkflowVBNet, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeDeploymentMergeModule, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeDeploymentCab, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeDeploymentSetup, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeDeploymentSmartDeviceCab, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeVSTA, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeVSTO, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeSharePointWorkflow, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeXNAWindows, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeXNAXBox, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeXNAZune, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeSharePointVBNet, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeSharePointCSharp, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeSilverlight, strcmp );
		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsProjectTypeAspNetMVCApplication, strcmp );

		HANDLE_STRING_COMPARE_RETURN( pszCompareString, vsConstVsTextView, strcmp );


#ifdef FTL_DEBUG
		if (!bFound)
		{
			FTLTRACEEX(FTL::tlWarning, TEXT("Unknown Guid String: %s\n"), COLE2T(bstring));
			pszResult = TEXT("Unknown:");
		}
		return pszResult;
#else
		FTLTRACEEX(FTL::tlWarning, TEXT("Unknown Guid String: %s\n"), COLE2T(bstring));
		return TEXT("Unknown");
#endif 
    }

	HRESULT CFDTEUtil::OpenDocumentAndGotoLine(DTE_NS::_DTE* pDTE, LPCTSTR pszFileName, int line, CComPtr<DTE_NS::Document>& spFileDocument)
	{
		HRESULT hr = E_FAIL;
		CHECK_POINTER_RETURN_VALUE_IF_FAIL(pDTE, E_INVALIDARG);
		//CComPtr< DTE_NS::Document > spFileDocument;

		if (pszFileName)
		{
			CComPtr< DTE_NS::Documents > spDocuments;
			COM_VERIFY(pDTE->get_Documents( &spDocuments ));
			CHECK_POINTER_RETURN_VALUE_IF_FAIL(spDocuments, hr);

			CComBSTR bstrFileName( pszFileName );
			CComVariant type=_T("Text");
			CComVariant read=_T("False");
			COM_VERIFY(spDocuments->Open( bstrFileName, 
				type.bstrVal,
				read.bVal, 
				&spFileDocument ));
		}
		else
		{
			COM_VERIFY(pDTE->get_ActiveDocument(&spFileDocument));
		}
		if ( !SUCCEEDED( hr ) || !spFileDocument)
		{
			return hr;
		}

		CComPtr< IDispatch > spSelectionDispatch;
		COM_VERIFY(spFileDocument->get_Selection( &spSelectionDispatch ));

		CComQIPtr< DTE_NS::TextSelection > spTextSelection(spSelectionDispatch);
		if (spTextSelection)
		{
			COM_VERIFY(spTextSelection->GotoLine( line, TRUE ));
		}

		COM_VERIFY(spFileDocument->Activate());
		//CComPtr<DTE_NS::Window> spMainWindow;
		//COM_VERIFY(pDTE->get_MainWindow(&spMainWindow));
		//if (SUCCEEDED(hr))
		//{
		//	COM_VERIFY(spMainWindow->Activate());
		//}

		return hr;
	}
    //////////////////////////////////////////////////////////////////////////

    HRESULT CFDTEDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("DTE")));

        if (m_pObj)
        {
            CComQIPtr<DTE_NS::_DTE>     spDTE(m_pObj);
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

				//DTE的主窗口，能通过其 get_HWnd 方法获取主窗口的HWnd句柄(这是唯一一个能获取到HWnd的 DTE_NS::Window 实例?)
				//  其他窗体的HWND必须通过 IVsTextView::GetWindowHandle 等来获取
				CComPtr<DTE_NS::Window>		spMainWindow;
				COM_VERIFY(spDTE->get_MainWindow(&spMainWindow));
				CFWindowDumper mainWindowDumper(spMainWindow, pInfoOutput, m_nIndent + 2);

				CComPtr<DTE_NS::Windows>    spWindows;
                COM_VERIFY(spDTE->get_Windows(&spWindows));
                CFWindowsDumper winsDumper(spWindows, pInfoOutput, m_nIndent + 2);

#ifdef ___DTE_FWD_DEFINED__
#pragma TODO("VSIP SDK dteinternal.h::_DTE::get_Solution is wrong, shold be get_Solution(_Solution**)")
				//CComPtr<DTE_NS::_Solution>  spSolution;
				//COM_VERIFY(spDTE->get_Solution(&spSolution));
				//COM_DETECT_INTERFACE_FROM_REGISTER(spSolution);
				//CFSolutionDumper solutionDumper(spSolution, pInfoOutput, m_nIndent + 2);
#else
				CComPtr<DTE_NS::_Solution>  spSolution;
				COM_VERIFY(spDTE->get_Solution(&spSolution));
				CFSolutionDumper solutionDumper(spSolution, pInfoOutput, m_nIndent + 2);
#endif
				CComPtr<DTE_NS::Document>   spActiveDocument;
				COM_VERIFY(spDTE->get_ActiveDocument(&spActiveDocument));
				CFDocumentDumper    activeDocumentDumper(spActiveDocument, pInfoOutput, m_nIndent + 2);

				CComPtr<DTE_NS::Documents>  spDocuments;
				COM_VERIFY(spDTE->get_Documents(&spDocuments));
				CFDocumentsDumper   documentsDumper(spDocuments, pInfoOutput, m_nIndent + 2);

				CComPtr<DTE_NS::AddIns> spAddins;
				COM_VERIFY(spDTE->get_AddIns(&spAddins));
				CFAddinsDumper addinsDumper(spAddins, pInfoOutput, m_nIndent + 2);

                CComPtr<DTE_NS::Events> spEvents;
                COM_VERIFY(spDTE->get_Events(&spEvents));
                CFEventsDumper      eventsDumper(spEvents, pInfoOutput, m_nIndent + 2);

				CComPtr<DTE_NS::Commands> spCommands;
				COM_VERIFY(spDTE->get_Commands(&spCommands));
				CFCommandsDumper commandsDumper(spCommands, pInfoOutput, m_nIndent + 2);
            }
        }
        return hr;
    }

	HRESULT CFAddinsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Addins")));
		if (m_pObj)
		{
			CComQIPtr<DTE_NS::AddIns>     spAddins(m_pObj);
			if (spAddins)
			{
				long nAddinsCount = 0;
				COM_VERIFY(spAddins->get_Count(&nAddinsCount));
				pInfoOutput->OnOutput(TEXT("Addin Count"), nAddinsCount);
				for (long nAddinIndex = 1; nAddinIndex <= nAddinsCount; ++nAddinIndex)
				{
					CComPtr<DTE_NS::AddIn> spAddin;
					COM_VERIFY(spAddins->Item(CComVariant(nAddinIndex), &spAddin));
					
					CFAddinDumper addinDumper(spAddin, pInfoOutput, m_nIndent + 2);
				}
			}
		}
		return hr;
	}

	HRESULT CFAddinDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Addin")));
		if (m_pObj)
		{
			CComQIPtr<DTE_NS::AddIn>     spAddin(m_pObj);
			if (spAddin)
			{
				CComBSTR bstrName;
				COM_VERIFY(spAddin->get_Name(&bstrName));
				pInfoOutput->OnOutput(TEXT("Name"), &bstrName);
				
				CComBSTR bstrDescription;
				COM_VERIFY(spAddin->get_Description(&bstrDescription));
				pInfoOutput->OnOutput(TEXT("Description"), &bstrDescription);

				VARIANT_BOOL bConnected;
				COM_VERIFY(spAddin->get_Connected(&bConnected));
				pInfoOutput->OnOutput(TEXT("Connected"), bConnected);

				CComBSTR bstrProgID;
				COM_VERIFY(spAddin->get_ProgID(&bstrProgID));
				pInfoOutput->OnOutput(TEXT("ProgID"), &bstrProgID);

				CComBSTR bstrGuid;
				COM_VERIFY(spAddin->get_Guid(&bstrGuid));
				pInfoOutput->OnOutput(TEXT("Guid"), &bstrGuid);

				CComBSTR bstrSatelliteDllPath;
				COM_VERIFY(spAddin->get_SatelliteDllPath(&bstrSatelliteDllPath));
				pInfoOutput->OnOutput(TEXT("SatelliteDllPath"), &bstrSatelliteDllPath);

				CComPtr<IDispatch> spDispatchObject;
				COM_VERIFY(spAddin->get_Object(&spDispatchObject));
				COM_DETECT_INTERFACE_FROM_REGISTER(spDispatchObject);
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
            CComQIPtr<DTE_NS::_Solution>     spSolution(m_pObj);
            if (spSolution)
            {
                CComBSTR bstrFileName;
                COM_VERIFY(spSolution->get_FileName(&bstrFileName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("FileName"), &bstrFileName));

                long nCount = 0;
                COM_VERIFY(spSolution->get_Count(&nCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Solution ProjectCount"), nCount));

                for (long nIndex = 1; nIndex <= nCount; nIndex++)
                {
                    CComPtr<DTE_NS::Project>    spProject;
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
            CComQIPtr<DTE_NS::Project>     spProject(m_pObj);
            if (spProject)
            {
                CComBSTR bstrName;
                COM_VERIFY(spProject->get_Name(&bstrName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

                CComBSTR bstrKind;
                COM_VERIFY(spProject->get_Kind(&bstrKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kind"), CFDTEUtil::GetDTEGuidStringInfo(bstrKind)));

                CComPtr<DTE_NS::ProjectItems>   spProjectItems;
                COM_VERIFY(spProject->get_ProjectItems(&spProjectItems));
                CFProjectItemsDumper projectItemsDumper(spProjectItems, pInfoOutput, m_nIndent + 2);

                CComPtr<DTE_NS::CodeModel>  spCodeModel;
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
            CComQIPtr<DTE_NS::ProjectItems>     spProjectItems(m_pObj);
            if (spProjectItems)
            {
                CComBSTR bstrKind;
                COM_VERIFY(spProjectItems->get_Kind(&bstrKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kind"), &bstrKind));

                long nCount = 0;
                COM_VERIFY(spProjectItems->get_Count(&nCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("ProjectItem Count"), nCount));

                for (long nIndex = 1; nIndex <= nCount; nIndex++)
                {
                    CComPtr<DTE_NS::ProjectItem>    spProjectItem;
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
            CComQIPtr<DTE_NS::ProjectItem>     spProjectItem(m_pObj);
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
					//代码(.cpp等) 能得到 VCFile、VSProjectItem、VCProjectItem 等
                    COM_DETECT_INTERFACE_FROM_REGISTER(spProjectItemModel);
                }
                
                CComPtr<DTE_NS::FileCodeModel> spFileCodeModel;
                COM_VERIFY_EXCEPT1(spProjectItem->get_FileCodeModel(&spFileCodeModel), S_FALSE);
                if (SUCCEEDED(hr) && spFileCodeModel)
                {
					//VC文件能得到 CodeElement、VCCodeElement、VCFileCodeModel、FileCodeModel2、IProvideMultipleClassInfo、VCDesignElement 等
                    COM_DETECT_INTERFACE_FROM_REGISTER(spFileCodeModel);
					CFFileCodeModelDumper fileCodeModelDumper(spFileCodeModel, pInfoOutput, m_nIndent + 2);
                }

                CComVariant varExtenderNames;
                COM_VERIFY(spProjectItem->get_ExtenderNames(&varExtenderNames));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("ExtenderNames"), &varExtenderNames));

				CComPtr<DTE_NS::Project>	spSubProject;
				COM_VERIFY(spProjectItem->get_SubProject(&spSubProject));
				if (SUCCEEDED(hr) && spSubProject)
				{
					pInfoOutput->OnOutput(TEXT("SubProject"));
					CFProjectDumper subProjectDumper(spSubProject , pInfoOutput, m_nIndent + 2);
					CComPtr<DTE_NS::ProjectItems> spSubProjectItems;
					COM_VERIFY(spSubProject->get_ProjectItems(&spSubProjectItems));
					if (SUCCEEDED(hr) && spSubProjectItems)
					{

					}
				}

                short sFileCount = 0;
                COM_VERIFY(spProjectItem->get_FileCount(&sFileCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("FileCount"), (long)sFileCount));

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
            CComQIPtr<DTE_NS::Events>     spEvents(m_pObj);
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
            CComQIPtr<DTE_NS::Windows>     spWindows(m_pObj);
            if (spWindows)
            {
                long nCount = 0;
                COM_VERIFY(spWindows->get_Count(&nCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Windows Count"), nCount));

                for (long index = 1; index <= nCount; index++)
                {
                    CComPtr<DTE_NS::Window> spWindow;
                    COM_VERIFY(spWindows->Item(CComVariant(index), &spWindow));
                    CFWindowDumper winDumper(spWindow, pInfoOutput, m_nIndent + 2);
                }
#ifdef ___DTE_FWD_DEFINED__
#else
                CComPtr<DTE_NS::_DTE> spDTE;
                COM_VERIFY(spWindows->get_DTE(&spDTE));
                CComPtr<DTE_NS::_DTE> spDTEParent;
                COM_VERIFY(spWindows->get_Parent(&spDTEParent));
                if (spDTE != spDTEParent)
                {
					FTLTRACEEX(FTL::tlWarning, TEXT("DTE and DTEParent is Not Same\n"));
                }
#endif 
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
            CComQIPtr<DTE_NS::Window>     spWindow(m_pObj);
            if (spWindow)
            {
                CComBSTR bstrCaption;
                COM_VERIFY(spWindow->get_Caption(&bstrCaption));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Caption"), &bstrCaption));
                
                VARIANT_BOOL bVisible = VARIANT_TRUE;
                COM_VERIFY(spWindow->get_Visible(&bVisible));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Visible"), bVisible));

                long nLeft = 0, nTop = 0, nWidth = 0, nHeight = 0;
                COM_VERIFY(spWindow->get_Left(&nLeft));
                COM_VERIFY(spWindow->get_Top(&nTop));
                COM_VERIFY(spWindow->get_Width(&nWidth));
                COM_VERIFY(spWindow->get_Height(&nHeight));

                CFStringFormater strPosition;
                strPosition.Format(TEXT("[%d,%d], {%d x %d}"), nLeft, nTop, nWidth, nHeight);
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("[left,top],{width x Height}"), strPosition.GetString()));

                DTE_NS::vsWindowType winType;
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
                    COM_DETECT_INTERFACE_FROM_REGISTER(spToolObject);
                }
                //Document 窗口可以获取 Selection
                //spWindow->get_Selection();
            }
        }
        return hr;
    }

    //Documents and Document
	HRESULT CFDocumentsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Documents")));

		if (m_pObj)
		{
			CComQIPtr<DTE_NS::Documents>     spDocuments(m_pObj);
			if (spDocuments)
			{
				long nDocumentCount = 0;
				COM_VERIFY(spDocuments->get_Count(&nDocumentCount));
				pInfoOutput->OnOutput(TEXT("Documents Count"), nDocumentCount);
				for (long nDocumentIndex = 1; nDocumentIndex <= nDocumentCount; ++nDocumentIndex)
				{
					CComPtr<DTE_NS::Document> spDocument;
					COM_VERIFY(spDocuments->Item(CComVariant(nDocumentIndex), &spDocument));
					CFDocumentDumper documentDumper(spDocument, pInfoOutput, m_nIndent + 2);
				}
			}
		}
		return hr;
	}

    HRESULT CFDocumentDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Document")));

        if (m_pObj)
        {
			CComQIPtr<DTE_NS::Document>     spDocument(m_pObj);
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
                COM_DETECT_INTERFACE_FROM_REGISTER(spSelection);
				//代码(如 .cpp) 返回的接口能转换为 TextSelection、LifetimeInformation 等
                CFTextSelectionDumper   selectionDumper(spSelection, pInfoOutput, m_nIndent + 2);

                CComPtr<DTE_NS::Window> spActiveWindow;
                COM_VERIFY(spDocument->get_ActiveWindow(&spActiveWindow));
                CFWindowDumper  activeWindowDumper(spActiveWindow, pInfoOutput, m_nIndent + 2);

				CComPtr<DTE_NS::ProjectItem> spProjectItem;
				COM_VERIFY(spDocument->get_ProjectItem(&spProjectItem));
				if (SUCCEEDED(hr) && spProjectItem)
				{
					CFProjectItemDumper	projectItemDumper(spProjectItem, pInfoOutput, m_nIndent + 2);
				}

                CComPtr<IDispatch> spTextDocumentDisp;
                COM_VERIFY(spDocument->Object(CComBSTR(L"TextDocument"),&spTextDocumentDisp));
                if (SUCCEEDED(hr) && spTextDocumentDisp)
                {
					CComQIPtr<DTE_NS::TextDocument> spTextDoc=spTextDocumentDisp;
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
            CComQIPtr<DTE_NS::TextDocument>     spTextDocument(m_pObj);
            if (spTextDocument)
            {
				CComBSTR bstrLanguage;
				COM_VERIFY(spTextDocument->get_Language(&bstrLanguage));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Language"), &bstrLanguage));
	
				CComBSTR bstrType;
				COM_VERIFY(spTextDocument->get_Type(&bstrType));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Type"), &bstrType));

				CComPtr<DTE_NS::TextPoint> spStartTextPoint;
                COM_VERIFY(spTextDocument->get_StartPoint(&spStartTextPoint));
				if (spStartTextPoint)
				{
					COM_VERIFY(pInfoOutput->OnOutput(TEXT("StartTextPoint")));
					CFTextPointDumper dndPointDumper(spStartTextPoint, pInfoOutput, m_nIndent + 2);
				}
                
				CComPtr<DTE_NS::TextPoint> spEndTextPoint;
                COM_VERIFY(spTextDocument->get_EndPoint(&spEndTextPoint));
				if (spEndTextPoint)
				{
					COM_VERIFY(pInfoOutput->OnOutput(TEXT("EndTextPoint")));
					CFTextPointDumper dndPointDumper(spEndTextPoint, pInfoOutput, m_nIndent + 2);
				}
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
            CComQIPtr<DTE_NS::TextSelection> spTextSelection(m_pObj);
            if (spTextSelection)
            {

            }
        }
        return hr;
    }

	HRESULT CFTextPointDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("TextPoint")));

		if (m_pObj)
		{
			CComQIPtr<DTE_NS::TextPoint> spTextPoint(m_pObj);
			if (spTextPoint)
			{
				long nLine = 0;
				COM_VERIFY(spTextPoint->get_Line(&nLine));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Line"), nLine));

				long nLineCharOffset = 0;
				COM_VERIFY(spTextPoint->get_LineCharOffset(&nLineCharOffset));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("LineCharOffset"), nLineCharOffset));

				long nAbsoluteCharOffset = 0;
				COM_VERIFY(spTextPoint->get_AbsoluteCharOffset(&nAbsoluteCharOffset));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("AbsoluteCharOffset"), nAbsoluteCharOffset));

				long nDisplayColumn = 0;
				COM_VERIFY(spTextPoint->get_DisplayColumn(&nDisplayColumn));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("DisplayColumn"), nDisplayColumn));

				long nLineLength = 0;
				COM_VERIFY(spTextPoint->get_LineLength(&nLineLength));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("LineLength"), nLineLength));

				CComPtr<DTE_NS::CodeElement> spCodeElement;
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
            CComQIPtr<DTE_NS::FileCodeModel>    spFileCodeModel(m_pObj);
            if (spFileCodeModel)
            {
                CComBSTR bstrLanguage;
                COM_VERIFY(spFileCodeModel->get_Language(&bstrLanguage));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Language"), CFDTEUtil::GetDTEGuidStringInfo(bstrLanguage)));

                CComPtr<DTE_NS::CodeElements>   spCodeElements;
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
            CComQIPtr<DTE_NS::CodeModel>     spCodeModel(m_pObj);
            if (spCodeModel)
            {
                CComBSTR bstrLanguage;
                COM_VERIFY(spCodeModel->get_Language(&bstrLanguage));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Language"), CFDTEUtil::GetDTEGuidStringInfo(bstrLanguage)));

                CComPtr<DTE_NS::CodeElements>   spCodeElements;
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
            CComQIPtr<DTE_NS::CodeElements>     spCodeElements(m_pObj);
            if (spCodeElements)
            {
                long nCount = 0;
                COM_VERIFY_EXCEPT1(spCodeElements->get_Count(&nCount), S_FALSE);
				if (SUCCEEDED(hr))
				{
					COM_VERIFY(pInfoOutput->OnOutput(TEXT("CodeElements Count"), nCount));
				}

                for (long nIndex = 1; nIndex <= nCount; nIndex++)
                {
                    CComPtr<DTE_NS::CodeElement> spCodeElement;
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
			//COM_DETECT_INTERFACE_FROM_REGISTER(m_pObj);

			CComQIPtr<DTE_NS::CodeElement>     spCodeElement(m_pObj);
            if (spCodeElement)
            {
				CComBSTR bstrFullName;
				COM_VERIFY(spCodeElement->get_FullName(&bstrFullName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("FullName"), &bstrFullName));

				CComBSTR bstrName;
                COM_VERIFY(spCodeElement->get_Name(&bstrName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

				VARIANT_BOOL isCodeType = VARIANT_FALSE;
				COM_VERIFY(spCodeElement->get_IsCodeType(&isCodeType));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("IsCodeType"), isCodeType));

                DTE_NS::vsCMElement elementKind = DTE_NS::vsCMElementOther;
                COM_VERIFY(spCodeElement->get_Kind(&elementKind));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Kind"), CFDTEUtil::GetElementKindString(elementKind)));

				CComBSTR bstrLanguage;
				COM_VERIFY(spCodeElement->get_Language(&bstrLanguage));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Language"), CFDTEUtil::GetDTEGuidStringInfo(bstrLanguage)));

				CComPtr<DTE_NS::TextPoint> spStartTextPoint;
				COM_VERIFY_EXCEPT1(spCodeElement->get_StartPoint(&spStartTextPoint), E_UNEXPECTED);
				CComPtr<DTE_NS::TextPoint> spEndTextPoint;
				COM_VERIFY_EXCEPT1(spCodeElement->get_EndPoint(&spEndTextPoint), E_UNEXPECTED);

				//CComVariant varExtenderNames;
				//COM_VERIFY(spCodeElement->get_ExtenderNames(&varExtenderNames));
				//if (SUCCEEDED(hr))
				//{
				//	COM_VERIFY(pInfoOutput->OnOutput(TEXT("ExtenderNames"), &varExtenderNames));
				//}

				CComBSTR bstrExtenderCATID;
				COM_VERIFY(spCodeElement->get_ExtenderCATID(&bstrExtenderCATID));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("ExtenderCATID"), CFDTEUtil::GetDTEGuidStringInfo(bstrExtenderCATID)));

				CComPtr<DTE_NS::CodeElements> spChildren;
				COM_VERIFY(spCodeElement->get_Children(&spChildren));
				if (SUCCEEDED(hr) && spChildren)
				{
					long nChildCount = 0;
					hr = spChildren->get_Count(&nChildCount);
					if (SUCCEEDED(hr) && nChildCount > 0)
					{
						COM_VERIFY(pInfoOutput->OnOutput(TEXT("Children")));
						CFCodeElementsDumper childrenDumper(spChildren, pInfoOutput, m_nIndent + 2);
					}
				}

#ifdef VCCML_NS & 0
				//VC Information
				CComQIPtr<VCCML_NS::VCCodeElement> spVCCodeElement(spCodeElement);
				if (spVCCodeElement)
				{
					CFVCCodeElementDumper	vcCodeElementDumper(spVCCodeElement, pInfoOutput, m_nIndent + 2);
				}
#endif
			}
        }
        return hr;
    }


#ifdef USE_MSC
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
#endif 


    HRESULT CFCommandsDumper::GetObjInfo(IInformationOutput* pInfoOutput)
    {
        HRESULT hr = E_POINTER;
        COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("Commands")));

        if (m_pObj)
        {
            CComQIPtr<DTE_NS::Commands>     spCommands(m_pObj);
            if (spCommands)
            {
                long lCommandCount = 0;
                COM_VERIFY(spCommands->get_Count(&lCommandCount));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Commands Count"), lCommandCount));

                for (long lCommandIndex = 1; lCommandIndex <= lCommandCount; ++lCommandIndex)
                {
                    CComPtr<DTE_NS::Command>  spCommand;
                    //TODO: ( pCommands->Item( CComVariant( "DoxygenAddin.Connect.DoxygenAddin" ), 0, &dx_cmd ), dx_cmd );
                    //  Need Text as Index ?
                    //用目前这种方式也可以获取，但ID有什么用？
                    COM_VERIFY(spCommands->Item(CComVariant(lCommandIndex), 0, &spCommand));
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
            CComQIPtr<DTE_NS::Command>     spCommand(m_pObj);
            if (spCommand)
            {
                CComBSTR bstrName;
                COM_VERIFY(spCommand->get_Name(&bstrName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

                CComBSTR bstrLocalizedName;
                COM_VERIFY(spCommand->get_LocalizedName(&bstrLocalizedName));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("LocalizedName"), &bstrLocalizedName));

#ifdef ___DTE_FWD_DEFINED__

#else
                CComBSTR bstrGuid;
                //Guid 有什么用？很多是一样，比如 File.FormProperties 和 Edit.ChangeBinding
                COM_VERIFY(spCommand->get_Guid(&bstrGuid));
                COM_VERIFY(pInfoOutput->OnOutput(TEXT("Guid"), &bstrGuid));
#endif

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

#ifdef VCCML_NS
	HRESULT CFVCFileCodeModelDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("VCFileCodeModel")));
		if (m_pObj)
		{
			CComQIPtr<VCCML_NS::VCFileCodeModel>     spVCFileCodeModel(m_pObj);
			if (spVCFileCodeModel)
			{
				CComPtr<DTE_NS::CodeElements> spAttributes;
				COM_VERIFY(spVCFileCodeModel->get_Attributes(&spAttributes));
				if (SUCCEEDED(hr) && spAttributes)
				{
					pInfoOutput->OnOutput(TEXT("VCFileCodeModel Attributes"));
					CFCodeElementsDumper attributesDumper(spAttributes, pInfoOutput, m_nIndent + 2);
				}

				CComPtr<DTE_NS::CodeElements> spClasses;
				COM_VERIFY(spVCFileCodeModel->get_Classes(&spClasses));
				if (SUCCEEDED(hr) && spClasses)
				{
					pInfoOutput->OnOutput(TEXT("VCFileCodeModel Classes"));
					CFCodeElementsDumper classesDumper(spClasses, pInfoOutput, m_nIndent + 2);
				}

				CComPtr<DTE_NS::CodeElements> spEnums;
				COM_VERIFY(spVCFileCodeModel->get_Enums(&spEnums));
				if (SUCCEEDED(hr) && spEnums)
				{
					pInfoOutput->OnOutput(TEXT("VCFileCodeModel Enums"));
					CFCodeElementsDumper enumsDumper(spEnums, pInfoOutput, m_nIndent + 2);
				}

				CComPtr<DTE_NS::CodeElements> spInterfaces;
				COM_VERIFY(spVCFileCodeModel->get_Interfaces(&spInterfaces));
				if (SUCCEEDED(hr) && spInterfaces)
				{
					pInfoOutput->OnOutput(TEXT("VCFileCodeModel Interfaces"));
					CFCodeElementsDumper interfacesDumper(spInterfaces, pInfoOutput, m_nIndent + 2);
				}

				//get_Macros, get_Maps

				CComPtr<DTE_NS::CodeElements> spFunctions;
				COM_VERIFY(spVCFileCodeModel->get_Functions(&spFunctions));
				if (SUCCEEDED(hr) && spFunctions)
				{
					pInfoOutput->OnOutput(TEXT("VCFileCodeModel Functions"));
					CFCodeElementsDumper functionsDumper(spFunctions, pInfoOutput, m_nIndent + 2);
				}

				CComPtr<DTE_NS::CodeElements> spVariables;
				COM_VERIFY(spVCFileCodeModel->get_Variables(&spVariables));
				if (SUCCEEDED(hr) && spVariables)
				{
					pInfoOutput->OnOutput(TEXT("VCFileCodeModel Variables"));
					CFCodeElementsDumper variablesDumper(spVariables, pInfoOutput, m_nIndent + 2);
				}

			}
		}
		return hr;
	}

	HRESULT CFVCCodeModelDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("VCCodeModel")));
		if (m_pObj)
		{
			CComQIPtr<VCCML_NS::VCCodeModel> spVCCodeModel(m_pObj);
			if (spVCCodeModel)
			{

			}
		}

		return hr;
	}

	HRESULT CFVCCodeElementDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		COM_VERIFY(pInfoOutput->OutputInfoName(TEXT("VCCodeElement")));
		if (m_pObj)
		{
			CComQIPtr<VCCML_NS::VCCodeElement> spVCCodeElement(m_pObj);
			if (spVCCodeElement)
			{
				CComBSTR	bstrName;
				COM_VERIFY(spVCCodeElement->get_Name(&bstrName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("Name"), &bstrName));

				CComBSTR	bstrFullName;
				COM_VERIFY(spVCCodeElement->get_FullName(&bstrFullName));
				COM_VERIFY(pInfoOutput->OnOutput(TEXT("FullName"), &bstrFullName));

				CComPtr<DTE_NS::CodeElements> spAttributes;
				COM_VERIFY_EXCEPT1(spVCCodeElement->get_Attributes(&spAttributes), S_FALSE);
				if (SUCCEEDED(hr) && spAttributes)
				{
					COM_VERIFY(pInfoOutput->OnOutput(TEXT("Attributes")));
					CFCodeElementsDumper attributesDumper(spAttributes, pInfoOutput, m_nIndent + 2);
				}

				CComPtr<VCCML_NS::VCCodeModel> spVCCodeModel;
				COM_VERIFY(spVCCodeElement->get_CodeModel(&spVCCodeModel));
				CFVCCodeModelDumper vcCodeModelDumper(spVCCodeModel, pInfoOutput, m_nIndent + 2);

				CComPtr<DTE_NS::CodeElements> spChildren;
				COM_VERIFY(spVCCodeElement->get_Children(&spChildren));
				if (SUCCEEDED(hr) && spChildren)
				{
					long nChildCount = 0;
					hr = spChildren->get_Count(&nChildCount);
					if (SUCCEEDED(hr) && nChildCount > 0)
					{
						COM_VERIFY(pInfoOutput->OnOutput(TEXT("VCCodeElement Children")));
						CFCodeElementsDumper childrenDumper(spChildren, pInfoOutput, m_nIndent + 2);
					}
				}

				CComQIPtr<VCCML_NS::VCCodeFunction> spVCCodeFunction(spVCCodeElement);
				if (spVCCodeFunction)
				{
					CFVCCodeFunctionDumper vcCodeFunctionDumper(spVCCodeFunction, pInfoOutput, m_nIndent + 2);
				}

			}
		}
		return hr;
	}

	HRESULT CFVCCodeFunctionDumper::GetObjInfo(IInformationOutput* pInfoOutput)
	{
		HRESULT hr = E_POINTER;
		pInfoOutput->OutputInfoName(TEXT("VCCodeFunction"));
		if (m_pObj)
		{
			CComQIPtr<VCCML_NS::VCCodeFunction> spVCCodeFunction(m_pObj);
			if (spVCCodeFunction)
			{
				DTE_NS::vsCMFunction functionKind = DTE_NS::vsCMFunctionOther;
				COM_VERIFY(spVCCodeFunction->get_FunctionKind(&functionKind));
				CFStringFormater functionKindFormater;
				CFDTEUtil::GetCMFunctionKindString(functionKindFormater, functionKind);
				pInfoOutput->OnOutput(TEXT("FunctionKind"), functionKindFormater.GetString());

				CComPtr<DTE_NS::CodeElements> spChildren;
				COM_VERIFY(spVCCodeFunction->get_Children(&spChildren));
				if (SUCCEEDED(hr) && spChildren)
				{
					long nChildCount = 0;
					hr = spChildren->get_Count(&nChildCount);
					if (SUCCEEDED(hr) && nChildCount > 0)
					{
						COM_VERIFY(pInfoOutput->OnOutput(TEXT("VCCodeFunction Children")));
						CFCodeElementsDumper childrenDumper(spChildren, pInfoOutput, m_nIndent + 2);
					}
				}
			}
		}
		return hr;
	}
#endif //VCCML_NS
}


#endif //FTL_FILE_HPP