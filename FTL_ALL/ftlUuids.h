#ifndef FTL_UUIDS_H
#define FTL_UUIDS_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlUuids.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
	*  
	*
	*****************************************************************************************************/

	class CFUuidsList
	{
		typedef struct {
			CHAR*   pszName;
			GUID    guid;
		} GUID_NAME_STRING_ENTRY;

	public:
		LPCSTR operator [] (const GUID& guid);
	protected:
		static GUID_NAME_STRING_ENTRY  s_GuidNames;
		static DWORD s_nGuidNameCount;
	};
}

#endif //FTL_UUIDS_H

#ifndef USE_EXPORT
#  include "ftlUuids.hpp"
#endif