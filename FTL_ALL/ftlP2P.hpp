#ifndef FTL_P2P_HPP
#define FTL_P2P_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlP2P.h"
#endif

namespace FTL
{
	CFP2PUtil::FreeData(LPCVOID pvData)
	{
		PeerFreeData(pvData);
	}

	CFP2PEnvInit::CFP2PEnvInit(BOOL bInitGraph /* = TRUE */, BOOL bInitPnrp /* = TRUE */)
	{
		m_bInitGraph = bInitGraph;
		m_bInitPnrp = bInitPnrp;
		//BOOL bStartup = FALSE;

		ZeroMemory(m_PeerVersion, sizeof(m_PeerVersion));
		HRESULT hr = ERROR_SERVICE_ALREADY_RUNNING;

		if (m_bInitPnrp)
		{
			COM_VERIFY(PeerGraphStartup(PEER_GRAPH_VERSION, &m_PeerVersion));
		}

		if (m_bInitPnrp)
		{
			COM_VERIFY(PeerPnrpStartup(PNRP_VERSION));
		}
		//m_bStartup = bStartup;
	}

	CFP2PEnvInit::~CFP2PEnvInit()
	{
		HRESULT hr = S_OK;
		if (m_bInitPnrp)
		{
			COM_VERIFY(PeerPnrpShutdown());
		}
		if (m_bInitGraph)
		{
			COM_VERIFY(PeerGraphShutdown());
		}
	}

	CFPeerGraph::CFPeerGraph()
	{
		m_hGrpah = NULL;
	}

	CFPeerGraph::~CFPeerGraph()
	{
		Close();
	}

	HRESULT CFPeerGraph::Create(PPEER_GRAPH_PROPERTIES pGraphProperties, PCWSTR pwzDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface)
	{
		FTLASSERT(NULL == m_hGrpah);
		HRESULT hr = E_FAIL;
		COM_VERIFY(PeerGraphCreate(pGraphProperties, pwzDatabaseName, pSecurityInterface, &m_hGrpah));
		return hr;
	}

	HRESULT CFPeerGraph::Open(PCWSTR pwzGraphId, PCWSTR pwzPeerId, PCWSTR pwzDatabaseName, 
		PPEER_SECURITY_INTERFACE pSecurityInterface, ULONG cRecordTypeSyncPrecedence, 
		const GUID *pRecordTypeSyncPrecedence, PHGRAPH phGraph)
	{
		FTLASSERT(NULL == m_hGrpah);

		HRESULT hr = E_FAIL;
		COM_VERIFY(PeerGraphOpen(pwzGraphId, pwzPeerId, pwzDatabaseName, pSecurityInterface, cRecordTypeSyncPrecedence, pRecordTypeSyncPrecedence,
			&m_hGrpah));
		return hr;
	}

	HRESULT CFPeerGraph::Close()
	{
		HRESULT hr = S_OK;
		if (m_hGrpah)
		{
			PeerGraphClose(m_hGrpah);
			m_hGrpah = NULL;
		}
	}

	CFPeerIdentity::CFPeerIdentity()
	{
		m_pwzIdentity = NULL;
	}
	CFPeerIdentity::~CFPeerIdentity()
	{
		Delete();
	}

	HRESULT CFPeerIdentity::Create(PCWSTR pwzClassifier, PCWSTR pwzFriendlyName, HCRYPTPROV hCryptProv)
	{
		FTLASSERT(NULL != m_pwzIdentity);

		HRESULT hr = E_FAIL;
		COM_VERIFY(PeerIdentityCreate(pwzClassifier, pwzFriendlyName, hCryptProv, &m_pwzIdentity));
		return hr;
	}

	HRESULT CFPeerIdentity::Delete()
	{
		HRESULT hr = S_OK;
		if (m_pwzIdentity)
		{
			COM_VERIFY(PeerIdentityDelete(m_pwzIdentity));
			m_pwzIdentity = NULL;
		}
		return hr;
	}

	PWSTR CFPeerIdentity::GetIdentity()
	{
		return m_pwzIdentity;
	}
}

#endif //FTL_P2P_HPP