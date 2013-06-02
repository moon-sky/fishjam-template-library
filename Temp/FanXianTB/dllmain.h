// dllmain.h : Declaration of module class.

class CFanXianTBModule : public CAtlDllModuleT< CFanXianTBModule >
{
public :
	DECLARE_LIBID(LIBID_FanXianTBLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FANXIANTB, "{6C8B185E-6448-457D-924D-8B7FEAF3F24E}")
};

extern class CFanXianTBModule _AtlModule;
