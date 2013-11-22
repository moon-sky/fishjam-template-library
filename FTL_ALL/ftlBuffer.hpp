#ifndef FTL_BUFFER_HPP
#define FTL_BUFFER_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlBuffer.h"
#endif

namespace FTL
{
	template <typename T>
	CFRWBufferT<T>::CFRWBufferT()
	{
		m_pbWriteBuffer = NULL;
		m_bWriteBufferIsAttached = FALSE;
		m_nWriteBufferSize = 0;
		m_nWriteBufferPos = 0;
		
		m_pbReadBuffer = NULL;
		m_bReadBufferIsAttached = FALSE;
		m_nReadBufferSize = 0;
		m_nReadBufferPos = 0;
		m_nReadBufferBytes = 0;
	}

	template <typename T>
	CFRWBufferT<T>::~CFRWBufferT()
	{
		if(!m_bWriteBufferIsAttached)
		{
			SAFE_DELETE_ARRAY(m_pbWriteBuffer);
		}
		if(!m_bReadBufferIsAttached)
		{
			SAFE_DELETE_ARRAY(m_pbReadBuffer);
		}
	}

	template <typename T>
	BOOL CFRWBufferT<T>::Attach(PBYTE pReadBuffer, LONG nReadBufferSize, PBYTE pWriteBuffer, LONG nWriteBufferSize)
	{
		BOOL bRet = TRUE;
		if(pReadBuffer && nReadBufferSize > 0)
		{
			if(!m_bReadBufferIsAttached)
			{
				API_VERIFY(SetReadBufferSize(0));
			}
			if(bRet)
			{
                LONG nDataSize = m_nReadBufferBytes - m_nReadBufferPos;
                if (m_pbReadBuffer && nDataSize > 0)
                {
                    //TODO: copy old data
                    //Checked::memcpy_s(m_pbReadBuffer, nReadSize, pbTemp + m_nReadBufferPos, nDataSize);
                }
				m_pbReadBuffer = pReadBuffer;
				m_nReadBufferSize = nReadBufferSize;
				m_bReadBufferIsAttached = TRUE;
			}
		}
		
		if(pWriteBuffer && nWriteBufferSize > 0)
		{
			if(!m_bWriteBufferIsAttached)
			{
				API_VERIFY(SetWriteBufferSize(0));
			}
            //TODO: Copy old data
			m_pbWriteBuffer = pWriteBuffer;
			m_nWriteBufferSize = nWriteBufferSize;
			m_bWriteBufferIsAttached = TRUE;
		}
		
		return bRet;
	}
	template <typename T>
    BOOL  CFRWBufferT<T>::Detach(PBYTE* ppReadBuffer, LONG* pReadBufferSize, PBYTE* ppWriteBuffer, LONG* pWriteBufferSize)
	{
		BOOL bRet = TRUE;
		
		return bRet;
	}


	template <typename T>
	BOOL CFRWBufferT<T>::SetReadBufferSize(LONG nReadSize)
	{
		BOOL bRet = TRUE;
        FTLASSERT(!m_bReadBufferIsAttached);

		//MS Bug: http://support.microsoft.com/kb/154895/en-us, 尝试更改掉,  (新版本已经没有了?)

		if (nReadSize != m_nReadBufferSize)
		{
			//计算没有被读取的数据大小
			LONG nDataSize = m_nReadBufferBytes - m_nReadBufferPos;
			FTLASSERT(nDataSize >= 0);

			if (nReadSize < nDataSize ) //m_dwReadBufferPos )
			{
				//还有大于设置的缓冲大小的数据没有被读取，没有办法保存
				bRet = FALSE;
			}
			else
			{
				if (nReadSize == 0)
				{
					//请求删除读取Buffer
					SAFE_DELETE_ARRAY(m_pbReadBuffer);
					m_nReadBufferPos = 0;
					m_nReadBufferBytes = 0;
				}
				else if (m_pbReadBuffer == NULL)
				{
					m_pbReadBuffer = new BYTE[nReadSize];
					m_nReadBufferPos = 0; //TODO: VC2008中是CInternetFile::SetReadBufferSize 实现里 dwReadSize, 好像是Bug
					m_nReadBufferBytes = 0;
				}
				else
				{
					//分配新的Buffer -- 先备份原来的指针，分配新的Buffer后拷贝数据
					LPBYTE pbTemp = m_pbReadBuffer;
					m_pbReadBuffer = new BYTE[nReadSize];

					//原来的Buffer中有数据，拷贝到新的Buffer中，
					if (nDataSize > 0) // && nDataSize <= nReadSize) //此处不用再和 nReadSize 比较 
					{
						//MS 这里有Bug，拷贝后 m_nReadBufferBytes 的值不对
						Checked::memcpy_s(m_pbReadBuffer, nReadSize, pbTemp + m_nReadBufferPos, nDataSize);
						m_nReadBufferPos = 0;
						m_nReadBufferBytes = nDataSize;
					}
					else
					{
						m_nReadBufferBytes = 0;
						m_nReadBufferPos = 0;//dwReadSize;
					}
					delete [] pbTemp;
				}

				m_nReadBufferSize = nReadSize;
			}
		}
		return bRet;
	}

	template <typename T>
	BOOL CFRWBufferT<T>::SetWriteBufferSize(LONG nWriteSize)
	{
		BOOL bRet = TRUE;
        FTLASSERT(!m_bWriteBufferIsAttached);

		if (nWriteSize != m_nWriteBufferSize)
		{
			if (m_nWriteBufferPos > nWriteSize)
			{
				//Buffer中已有数据比新的Buffer大
				API_VERIFY(FlushFromBuffer(NULL));
				if (!bRet)
				{
					return bRet;
				}
			}
			if (nWriteSize == 0)
			{
				SAFE_DELETE_ARRAY(m_pbWriteBuffer);
				m_nWriteBufferPos = 0;
			}
			else if (m_pbWriteBuffer == NULL)
			{
				m_pbWriteBuffer = new BYTE[nWriteSize];
				m_nWriteBufferPos = 0;
			}
			else
			{
				//备份以前的数据
				LPBYTE pbTemp = m_pbWriteBuffer;
				m_pbWriteBuffer = new BYTE[nWriteSize];
				FTLASSERT(m_nWriteBufferPos <= nWriteSize);

				Checked::memcpy_s(m_pbWriteBuffer, nWriteSize, pbTemp, m_nWriteBufferPos);
				delete [] pbTemp;
			}

			m_nWriteBufferSize = nWriteSize;
		}

		return bRet;
	}

	template <typename T>
	BOOL CFRWBufferT<T>::ReadFromBuffer(PBYTE pBuffer, LONG nCount, LONG* pRead)
	{
		LONG nBytes = 0;
		BOOL bRet = TRUE;

		T* pT = static_cast<T*>(this);

		if (m_pbReadBuffer == NULL)
		{
			//没有缓冲，直接处理
			API_VERIFY(pT->ReadReal(pBuffer, nCount, &nBytes));
			//API_VERIFY(InternetReadFile(m_hRequest, (LPVOID) pBuffer, nCount, &nBytes));
			if (bRet && pRead)
			{
				*pRead = nBytes;
			}
			return bRet;
		}

		//如果请求Buffer大于缓冲Buffer
		if (nCount >= m_nReadBufferSize)
		{
			//先拷贝缓冲中的数据
			LONG nDataSize = m_nReadBufferBytes - m_nReadBufferPos;
			FTLASSERT(nDataSize >= 0);
			if(nDataSize > 0)
			{
				Checked::memcpy_s(pBuffer, nCount, m_pbReadBuffer + m_nReadBufferPos, nDataSize);
				//此时有效数据已经全部读取完
			}
			m_nReadBufferPos = 0;
			m_nReadBufferBytes = 0;

			//继续从网络读取数据
			API_VERIFY(pT->ReadReal(pBuffer + nDataSize, nCount - nDataSize, &nBytes));
			//API_VERIFY(InternetReadFile(m_hRequest, pBuffer + nDataSize, nCount - nDataSize, &nBytes));
			if (!bRet)
			{
				return bRet;
			}
			nBytes += nDataSize;
		}
		else  //请求Buffer小于缓冲Buffer，优先从缓冲Buffer中读取
		{
			//如果读Buffer中剩余的数据比请求的数据少 -- 先将读Buffer中的全部数据都拷贝出来
			LONG nDataSize = m_nReadBufferBytes - m_nReadBufferPos;
			if (nDataSize <= nCount)  //			if (m_nReadBufferPos + nCount >= m_nReadBufferBytes)
			{
				if (nDataSize > 0)
				{
					Checked::memcpy_s(pBuffer, nCount, m_pbReadBuffer + m_nReadBufferPos, nDataSize);
				}
				m_nReadBufferPos = 0;
				m_nReadBufferBytes = 0;

				//继续从网络读取数据 -- 这次直接读取到缓冲Buffer中，而且因为读Buffer中的有效数据已经全部读完，则从头开始缓冲
				API_VERIFY(pT->ReadReal(m_pbReadBuffer, m_nReadBufferSize, &nBytes));
				//API_VERIFY(InternetReadFile(m_hRequest, m_pbReadBuffer, m_nReadBufferSize, &nBytes));
				if (bRet)
				{
					//增加刚读取到的数据到请求Buffer
					LONG nMoveSize = FTL_MIN(nCount - nDataSize, nBytes); 
					Checked::memcpy_s(pBuffer + nDataSize, nCount - nDataSize, 
						m_pbReadBuffer, nMoveSize);
					m_nReadBufferPos = nMoveSize;
					m_nReadBufferBytes = nBytes;

					nBytes = (nMoveSize + nDataSize);
				}
			}
			else
			{
				//如果读Buffer中剩余的数据比请求的数据多，则本次只在内存中拷贝
				Checked::memcpy_s(pBuffer, nCount, m_pbReadBuffer + m_nReadBufferPos, nCount);
				m_nReadBufferPos += nCount;
				nBytes = nCount;
			}
		}
		if (pRead)
		{
			*pRead = nBytes;
		}
		return bRet;
	}

	template<typename T>
	BOOL CFRWBufferT<T>::WriteToBuffer(const PBYTE pBuffer, LONG nCount, LONG* pWrite)
	{
		BOOL bRet = TRUE;
		LONG nBytes = 0;
		T* pT = static_cast<T*>(this);

		if (m_pbWriteBuffer == NULL)
		{
			API_VERIFY(pT->WriteReal(pBuffer, nCount, &nBytes));
			//没有缓冲区，直接发送
			//API_VERIFY(InternetWriteFile(m_hRequest, pBuffer, nCount, &nBytes));
			if (!bRet)
			{
				return bRet;
			}
			FTLASSERT(nCount == nBytes);
		}
		else
		{
			//使用缓冲区
			if ((m_nWriteBufferPos + nCount) > m_nWriteBufferSize && m_nWriteBufferPos > 0)
			{
				//如果 旧的数据长度 + 新的数据长度 比 缓冲区的大小大，则马上把旧的数据发出去，并重设Pos
				API_VERIFY(pT->WriteReal(m_pbWriteBuffer,	m_nWriteBufferPos, &nBytes));
				//API_VERIFY(InternetWriteFile( m_hRequest, m_pbWriteBuffer,	m_nWriteBufferPos, &dwBytes));
				if (!bRet)
				{
					return bRet;
				}
				//FTLASSERT(nCount == dwBytes);
				m_nWriteBufferPos = 0;
			}

			//如果需要发送的数据比缓冲区的还大，则直接发送；否则进行缓冲
			if (nCount >= m_nWriteBufferSize)
			{
				API_VERIFY(pT->WriteReal(pBuffer, nCount, &nBytes));
				//API_VERIFY(InternetWriteFile(m_hRequest, (LPVOID) pBuffer, nCount, &dwBytes));
			}
			else
			{
				FTLASSERT(m_nWriteBufferPos + nCount <= m_nWriteBufferSize);
				//if (m_nWriteBufferPos + nCount < m_nWriteBufferSize)
				Checked::memcpy_s(m_pbWriteBuffer + m_nWriteBufferPos, 
					m_nWriteBufferSize - m_nWriteBufferPos, pBuffer, nCount);
				m_nWriteBufferPos += nCount;
			}
		}

		if (pWrite)
		{
			*pWrite = nBytes;
		}
		return bRet;
	}

	template <typename T>
	BOOL CFRWBufferT<T>::FlushFromBuffer(LONG* pWrite)
	{
		BOOL bRet = TRUE;
		if (m_pbWriteBuffer != NULL && m_nWriteBufferPos > 0)
		{
			T* pT = static_cast<T*>(this);

			LONG nBytes = 0;
			API_VERIFY(pT->WriteReal(m_pbWriteBuffer, m_nWriteBufferPos, &nBytes));
			//API_VERIFY(InternetWriteFile(m_hRequest, m_pbWriteBuffer, m_nWriteBufferPos, &dwBytes));
			if (!bRet)
			{
				return bRet;
			}
			//FTLASSERT( dwBytes == m_nWriteBufferPos);
			m_nWriteBufferPos = 0;

			if (pWrite)
			{
				*pWrite = nBytes;
			}
		}
		return bRet;
	}
}

#endif //FTL_BUFFER_HPP