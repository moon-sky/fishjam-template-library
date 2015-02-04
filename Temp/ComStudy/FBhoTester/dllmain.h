// dllmain.h : Declaration of module class.

class CFBhoTesterModule : public CAtlDllModuleT< CFBhoTesterModule >
{
public :
	DECLARE_LIBID(LIBID_FBhoTesterLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_FBHOTESTER, "{8BB0CF1E-6167-4DFB-8D6D-3A4A52E04503}")
};

extern class CFBhoTesterModule _AtlModule;
