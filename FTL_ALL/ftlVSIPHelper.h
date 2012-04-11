#ifndef FTL_VSIP_HELPER_H
#define FTL_VSIP_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlVSIPHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlVSIPHelper.h requires ftlCom.h to be included first
#endif

/********************************************************************************************
* #define NOMINMAX // Windows Platform min and max macros cause problems for the Standard C++ Library
*
* __if_exists(Derived_T::PreCreate)
* 实现 XXXImpl 接口父类最简单的方法：参见 ISupportErrorInfoImpl
* OBJECT_ENTRY_AUTO(COM对象的CLSID, COM对象对应的类) -- 通过 DllGetClassObject 导出COM对象的实现方法,
* 模板实现中，可以用如下方式根据子类是否实现了制定的方法来条件调用
*   Derived_T& rDerived = *(static_cast<Derived_T*>(this));   //获取模板实现子类的实例
*   __if_exists(Derived_T::XXXX) { rDerived.XXXX(); }         //判断模板实现子类是否定义了对应的方法，并条件调用
* 
* IDL 文件中
*   idl格式： [属性] interface 接口名 { [方法属性] 方法名([参数属性] 参数); } 
*   importlib(".tlb/.olb")--在 library 中导入指定的类型库信息?和import的区别?
*   midl_pragma warning -- 指定编译时警告信息，如 (disable : 2015) / (default : 2015)
*   nonextensible -- 
*   属性：
*     propget
*     propput
*     bindable
*     requestedit
*     id
*     retval -- ？说明是返回值，如 BSTR* pName, 问题：？Count([out, retval] short* piCountOfItems)
********************************************************************************************/

/********************************************************************************************
*   
* 接口实现类(VSL::)
*   IOleCommandTargetImpl -- 实现 IOleCommandTarget 接口，能处理菜单或工具条事件
*   IServiceProviderImpl -- 提供Service，使用 BEGIN_SERVICE_MAP、SERVICE_ENTRY 等宏设置能提供的Service
*   ISupportErrorInfoImpl -- 模板参数一般是 InterfaceSupportsErrorInfoList< IXXXX, ErrorInfoList的递归 >
*   IVsInstalledProductImpl -- 在Splash窗体中显示插件信息(就不再需要 .rgs 中的信息?)，模板参数为相关的资源ID，在.rgs文件中还需要 InstalledProducts 项
*   IVsPackageImpl -- 实现 IVsPackage 接口, COM组件成为 VS Package
*   IVsUIHierarchyImpl -- 树状结构(如Solution Explorer的实现类)，实现了 IVsHierarchy 和 IVsUIHierarchy 两个接口
* 
* VSL中的类
*   VsSiteCache(服务的Cache,提供 QueryService ) -- VsSiteCacheGlobal、VsSiteCacheLocal
*     ？QueryService时一般使用 xxxPackageVsSiteCache.GetCachedService<IVsXXXX, SID_SVsXXX>() 的方式调用，SID_ 后面是C#中对应的字符
*     ToolWindowBase 中有 GetVsSiteCache/GetPackageVsSiteCache， 两个有什么区别？从全局如何访问？
*   StaticallySizedIconImageListWin32Control -- 图标列表? 需要定义一个 enum{xxx1,xxx2, CountOfImages};
*     和一个 static const WORD m_sResourceIDs[CountOfImages] = { IDI_XXX, } 的数组
*   ToolWindowBase -- 创建 ToolWindow 时的基类，实现了
*     一般需要实现 PostCreate 方法，将窗体加入 VS Frame(有没有自动方法？至少 ExtensibilityExplorer 中是手动实现的)
*   LoadUILibrary::ExtendedErrorInfo -- 在Package静态的 GetLoadUILibraryErrorInfo 中提供错误信息
*   VSL::Pointer -- 
*   NonCocreateableComObject -- 可用 CreateInstance 创建COM对象，和 CComObject 的区别?
* 
* 可直接使用的全局工具类，一般是对应接口的全局实例的封装，有可直接调用的静态方法
*   VsIServiceProviderUtilities<> -- 对 IServiceProvider 的封装，可直接调用其静态的 GetCachedService<IVsShell, SID_SVsShell>() 方法
*   VsUIShellUtilities<> -- 对 IVsUIShell 的封装
*   OleComponentUIManagerUtilities<> -- 对 IOleComponentUIManager 的封装
*
*
* 宏
*   1.命令处理映射(IOleCommandTargetImpl)， 采用 GUID/DWORD 对的方式进行映射处理：
*     VSL_BEGIN_COMMAND_MAP
*       VSL_COMMAND_MAP_ENTRY(vsct中使用的GUID, 命令ID, 查询状态回调, 执行回调 )
*       VSL_COMMAND_MAP_CLASS_ENTRY( 使用从 CommandHandler 继承的子类动态映射 )
*     VSL_END_VSCOMMAND_MAP
*     系统自带的 GUID/命令ID对 有 GUID_VsUIHierarchyWindowCmds/UIHWCMDID_RightClick 等
*   2.工具映射(ToolMap), 实现 IVsPackage::CreateTool 方法，在VS需要时创建对应的 Tool, Tool 需要在注册表的 ToolWindows 下注册
*     VSL_BEGIN_TOOL_MAP -- VSL_TOOL_ENTRY(对应的GUID, 创建回调 )
*   3.注册表资源映射( 提供.rgs中通过 %token% 使用的变量值 ? )
*     VSL_BEGIN_REGISTRY_MAP(.rgs文件对应的资源ID)
*       VSL_REGISTRY_MAP_GUID_ENTRY( 提供 .rgs 文件中使用的 CLSID 值 )
*       VSL_REGISTRY_MAP_NUMBER_ENTRY( 可提供 PLK -- 如 IDS_XXXX_LOAD_KEY ) 
*       VSL_REGISTRY_RESOURCE_STRING_ENTRY( 提供字符串资源如PackageName )
*     VSL_END_REGISTRY_MAP
*   4.PLK 映射(似乎已经由 VSL_REGISTRY_MAP_NUMBER_ENTRY 替代 ?)
*     VSL_DECLARE_REGISTRY_RESOURCEID(IDS_XXXX_LOAD_KEY)
*        ...
*     VSL_REGISTRY_RESOURCEID_ENTRY(IDS_XXXX_LOAD_KEY)
*     
* 
* 常用的系统常量
*   CLSID_VSUIHIERARCHYWINDOW -- VS 提供的 hierarchy tool window，可在 ToolWindowBase子类的GetLocalRegistryCLSIDViewObject
*     中返回表tool window 的类型?
*
* 常见服务(Service)
*   SID_SLocalRegistry(ILocalRegistry4) -- 
*
* Hierarchy(层次结构)
*   IVsHierarchy / IVsUIHierarchy
*   自定义 Hierarchy 结构时可以从 IVsUIHierarchyItem/IVsHierarchyItem 继承?
*
* 错误处理
*   VSL_CREATE_ERROR_HRESULT -- 会 throw exception ?
*   VSL_STDMETHODTRY {...} VSL_STDMETHODCATCH()<CR> return VSL_GET_STDMETHOD_HRESULT(); -- 在返回 HRESULT 的函数中的 try..catch 结构
*  
********************************************************************************************/

/********************************************************************************************
* RGS配置信息 (%REGROOTBEGIN% -- %REGROOTEND%)
*   Menus -- 
*   Packages -- 
*     CLSID_XXX = s '名字'
*   Services
*     IID_YYY = s '%CLSID_XXX%'
********************************************************************************************/

/********************************************************************************************
* VS的系统Frame的GUID,可通过 VsUIShellUtilities<>::FindToolWindow(FTW_fForceCreate, ...) 获取
* 通过 CFVsUIShellDumper::GetToolWindowEnum 获取的 IVsWindowFrame 中的 VSFPROPID_GuidPersistenceSlot 属性)
* 也可通过注册表中查找( VisualStudio\版本\Configuration\ToolWindows )
*   Macro Explorer -- {07CD18B4-3BA1-11D2-890A-0060083196C6}
*   Object Browser -- {269A02DC-6AF8-11D3-BDC4-00C04F688E50}
*   Output -- {34E76E81-EE4A-11D0-AE2E-00A0C90FFFC3}
*   Properties -- {EEFA5220-E298-11D0-8F78-00A0C9110057}
*   Property Manager -- {DE1FC918-F32E-4DD7-A915-1792A051F26B}
*   Solution Explorer -- {3AE79031-E1BC-11D0-8F78-00A0C9110057}
********************************************************************************************/
namespace FTL
{
    class CFVSIPUtils
    {
    public:
		//Hierarchy property Id -- VSHPROPID_
		FTLINLINE static LPCTSTR GetVSHPropIdString(DWORD_PTR propId);
		
		//Frame(Window/Document/ToolWidndow) property Id -- VSFPROPID_
		FTLINLINE static LPCTSTR GetVSFPropIdIdString(DWORD_PTR propId);

        //获取 cmdidcmd.h 中定义的 CmdID 对应的字符串，在 IOleCommandTarget::QueryStatus 中判断 cmds[n].cmdID
        FTLINLINE static LPCTSTR GetStdIdCommandtring(ULONG cmdID);

		FTLINLINE static BOOL IsVsHierarchyHasChildren(IVsHierarchy* pParent, VSITEMID ItemId);
    };

	class CFVsPackageDumper : public CFInterfaceDumperBase<CFVsPackageDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsPackageDumper);
	public:
		FTLINLINE explicit CFVsPackageDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsPackageDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsInstalledProductDumper : public CFInterfaceDumperBase<CFVsInstalledProductDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsInstalledProductDumper);
	public:
		FTLINLINE explicit CFVsInstalledProductDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsInstalledProductDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsUIShellDumper : public CFInterfaceDumperBase<CFVsUIShellDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsUIShellDumper);
	public:
		FTLINLINE explicit CFVsUIShellDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsUIShellDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFEnumWindowFramesDumper : public CFInterfaceDumperBase<CFEnumWindowFramesDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFEnumWindowFramesDumper);
	public:
		FTLINLINE explicit CFEnumWindowFramesDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFEnumWindowFramesDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};
	

	class CFVsTextManagerDumper : public CFInterfaceDumperBase<CFVsTextManagerDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextManagerDumper);
	public:
		FTLINLINE explicit CFVsTextManagerDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextManagerDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextMarkerTypeDumper : public CFInterfaceDumperBase<CFVsTextMarkerTypeDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextMarkerTypeDumper);
	public:
		FTLINLINE explicit CFVsTextMarkerTypeDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextMarkerTypeDumper>(pObj, pInfoOutput, nIndent){}
	public:
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//Solution and Projects
	class CFVsSolutionDumper : public CFInterfaceDumperBase<CFVsSolutionDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsSolutionDumper);
	public:
		FTLINLINE explicit CFVsSolutionDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsSolutionDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//WindowFrame -> IVsWindowPane/CodeWindow/TextView -> TextBuffer(Colorizer) 
    //  Type: 
    //    ToolWindow -- 通常单实例,有 IOleCommandTarget/IVsWindowPane 等，要在 ToolWindows/Menus 注册表下注册
    //    DocumentWindow(EditorWindow) -- 通常多实例，关联Document
    class CVsWindowFrameDumper : public CFInterfaceDumperBase<CVsWindowFrameDumper>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CVsWindowFrameDumper);
    public:
        FTLINLINE explicit CVsWindowFrameDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
            :CFInterfaceDumperBase<CVsWindowFrameDumper>(pObj, pInfoOutput, nIndent){}
    public:
        //override
        FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
    };

	class CFVsHierarchyDumper : public CFInterfaceDumperBase<CFVsHierarchyDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsHierarchyDumper);
	public:
		FTLINLINE explicit CFVsHierarchyDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsHierarchyDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	private:
		//BOOL    HasChildren(IVsHierarchy* pParent, VSITEMID ItemId);
		HRESULT DumpAllPropertiesInfo(IVsHierarchy* pParent, VSITEMID ItemId, IInformationOutput* pInfoOutput);
		HRESULT EnumAllChildRen(IVsHierarchy* pParent,VSITEMID startItemId, IInformationOutput* pInfoOutput);
	};

	//Window
	class CFVsWindowFrameDumper : public CFInterfaceDumperBase<CFVsWindowFrameDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsWindowFrameDumper);
	public:
		FTLINLINE explicit CFVsWindowFrameDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsWindowFrameDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsTextViewDumper : public CFInterfaceDumperBase<CFVsTextViewDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsTextViewDumper);
	public:
		FTLINLINE explicit CFVsTextViewDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsTextViewDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFVsUIHierarchyDumper : public CFInterfaceDumperBase<CFVsUIHierarchyDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFVsUIHierarchyDumper);
	public:
		FTLINLINE explicit CFVsUIHierarchyDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFVsUIHierarchyDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//Registry
	class CFLocalRegistryDumper : public CFInterfaceDumperBase<CFLocalRegistryDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFLocalRegistryDumper);
	public:
		FTLINLINE explicit CFLocalRegistryDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFLocalRegistryDumper>(pObj, pInfoOutput, nIndent){}
		//override
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlVSIPHelper.hpp"
#endif

#endif //FTL_VSIP_HELPER_H
