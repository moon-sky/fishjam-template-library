#ifndef FTL_UUIDS_HPP
#define FTL_UUIDS_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlUuids.h"
#endif

namespace FTL
{

#define OUR_GUID_ENTRY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	{ #name, { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } } },

	__declspec(selectany) CFUuidsList::GUID_NAME_STRING_ENTRY CFUuidsList::s_GuidNames[] = {
		#include <uuids.h>
	};

	__declspec(selectany) CFUuidsList::s_nGuidNameCount = CFUuidsList::_countof(s_GuidNames);
	TCHAR* CFUuidsList::operator [](const GUID& guid)
	{
		if (guid == GUID_NULL) {
			return TEXT("GUID_NULL");
		}

		for (DWORD i = 0; i < s_nGuidNameCount; i++) {
			if (s_GuidNames[i].guid == guid) {
				return s_GuidNames[i].szName;
			}
		}

		return NULL;
	}
}

#endif //FTL_UUIDS_HPP