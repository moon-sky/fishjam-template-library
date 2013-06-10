// dllmain.h : Declaration of module class.

#include "FanXianIEPlugin_i.h"
class CFanXianIEPluginModule : public CAtlDllModuleT< CFanXianIEPluginModule >
{
public :
	DECLARE_LIBID(LIBID_FanXianIEPluginLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FANXIANIEPLUGIN, "{6C8B185E-6448-457D-924D-8B7FEAF3F24E}")

	HMODULE		GetModuleInstance(){ return m_hModuleInstance; }
	HMODULE		m_hModuleInstance;
};

extern class CFanXianIEPluginModule _AtlModule;
