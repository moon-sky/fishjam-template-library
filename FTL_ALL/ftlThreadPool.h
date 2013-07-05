///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlThreadPool.h
/// @brief  Fishjam Template Library ThreadPool Header File.
/// @author fujie
/// @version 1.0
/// @date 03/30/2008  -- first version
/// @defgroup ftlThreadPool ftl thread pool function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************************************************************
* �Ѷ�����
*   C++\ATL\Advanced\CThreadPool -- ʹ��ATL�е� CThreadPool ������
*************************************************************************************************************************/

#ifndef FTL_THREADPOOL_H
#define FTL_THREADPOOL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlThreadPool.h requires ftlbase.h to be included first
#endif

#include "ftlThread.h"
//#include "ftlSharePtr.h"
//#include "ftlFunctional.h"

namespace FTL
{
	/*********************************************************************************************
    * �̳߳� -- ����Ԥ�������������̴߳��������ٱ����������Ŀ�����̯���˸�������������ϣ�
    *           ִ�д���Խ�࣬ÿ���������ֵ������̱߳���������ԽС
    *   ���÷�Χ��
    *     1.��λʱ���ڴ�������Ƶ������������ʱ��� -- �粻�ʺ� FTP ��������
    *     2.��ʵʱ��Ҫ��ϸߡ�������ܵ�������ڴ����̣߳��������㲻��ʵʱҪ����˱�������̳߳ؽ���Ԥ������ 
    *     ע�⣺ʹ���̳߳��������Ŀ����ϴ�,�൱����̺߳��ڲ����ݽṹ�������Ľ��̵�һ����ɲ���,��ҪäĿʹ��
	*
	* ΢���ṩ���̳߳�(ms-help://MS.MSDNQTR.v90.en/dllproc/base/thread_pools.htm) -- Vista֮�����?
	*	�̳߳������ĸ������ÿ�������һ���߳����
	*     1. �� I/O ��� ( WT_EXECUTEDEFAULT )�������߳�����ɶ˿����������˿���ͬʱ���е��߳���������Ϊ CPU ������������
	*        �� I/O ����е��߳��Ǹ��ݹ����������������ġ�
	*     2. I/O ���( WT_EXECUTEINIOTHREAD ) ���� ���з����첽I/O����Ĵ���
	*        CreateThreadpoolIo -> StartThreadpoolIo -> 
	*     3. ��ʱ����� ( WT_EXECUTEINTIMERTHREAD ) -- ʹ��ʱ��������߳��ܹ�ִ�иù�����Ŀ,ע��:����Ӧ��Ѹ��ִ�У���Ӧ���ж�
	*     4. �ȴ���� (WT_EXECUTEINWAITTHREAD) -- ����ʹ�ȴ�������߳�֮һ���й�����Ŀ�������������ʸ���(������Ŀ�����Ŷӷ��� I/O �����)��
	*      ����һ��Σ��(����ִ�й�����Ŀ�ĵȴ�����������߳��޷��ȴ���������õ�֪ͨ)
	* 
	*   �ܹ�ִ�����в���
	*     a.�첽���ú���(�������������̷߳���ÿһ���ͻ�)
	*       QueueUserWorkItem(�첽���ú���) -- ��һ��������Ŀ(�ص������Ķ���)�Ŷӷ����̳߳��е�һ���߳��в����������أ������Ͳ���ȡ��
	*         �̳߳��е�ĳ���߳̽������ù�����Ŀ�����º��������á��߳�ִ����󲢲����������������Ƿ����̳߳ء�
	*         ����Ϊÿ���ͻ������󴴽��ͳ����̡߳��ڲ�ʹ�� I/O ��ɶ˿�
	*     b.���չ涨��ʱ�������ú��� -- �ȴ���ʱ���ں˶���,����ػ�û���ʱ���֪ͨ.�̳߳صĶ�ʱ����������ȴ���ʱ��,��APC��Ŀ�Ŷ�
	*       CreateTimerQueue -- ������ʱ������,��һ�鶨ʱ��������֯����
	*       CreateTimerQueueTimer -- �ڶ�ʱ�������д�����ʱ��,����ָ������ϵͳĬ�ϵĶ�ʱ������(NULL)
	*       DeleteTimerQueueTimer/DeleteTimerQueue -- ��ʹ��ʱ��Ҫɾ�� ��ʱ��/��ʱ������ ,ע��:��Ҫ�ڹ�����Ŀ�߳���ɾ��,��������������
	*       ChangeTimerQueueTimer -- �ı䶨ʱ������ʱ��͵�������,�������ڵ�����ʱ����Ŀ����ʱ,����ʱ��
	*         �����ڵ���ʱ�ر��� MessageBox �����ĶԻ��� --WM_TIMER Ҳ����,�ؼ�����ʾ��Ϣ�� ID Ϊ0x0000FFFF(Spy++)
	*     c.�������ں˶����Ϊ��֪ͨ״̬ʱ���ú��� -- ���͸��ȴ����(��������Ŀ׼��ִ��ʱ������Ĭ���Ŷӷ���� I/O ������߳���)
	*       RegisterWaitForSingleObject -- �ڲ�ʹ�� WaitForMultipleObjects ���ȴ��Ѿ�ע��Ķ���
	*         ���ƣ�1.���ܶ�εȴ��������(DuplicateHandle)
	*               2.ÿ�� 63 ������󣬾�Ҫ����һ���߳����Ӹ������
	*       UnregisterWait -- ȡ��ע��״̬.��ȡ���ȴ������ע��״̬֮ǰ����Ҫ�ر��ں˶���ľ�����ᵼ�������ȴ�������޷���������
	*     d.���첽 I/O �������ʱ���ú���
	*       ������Ӧ�ó��򷢳�ĳЩ�첽 I/O ���󣬵���Щ�������ʱ����Ҫ��һ���̳߳�׼��������������ɵ�I/O����(���豸���������̳߳صķ�I/O�����������)��
	*       BindIoCompletionCallback -- �����첽IO������̳߳��еĺ���ָ�룬�ں����� ReadFileEx/WriteFileEx �Ⱥ������ʱ������ָ���ĺ�����
	*         �ú������ڲ�����CreateIoCompletionPort�����Ա�֤������һ���߳�ʼ���ڷ�I/O�����.
	*         ���Ҫ���������������Ϣ����ص�������Ӧ��ʹ�ý���������Ϣ���� OVERLAPPED �ṹ�Ľ�β���Ĵ�ͳ����(���� OVERLAPPED Ϊ��һ����Ա���������µĽṹ��)
	*         ע�⣺�ر��豸�ᵼ���������д�������I/O����������ɣ�������һ��������롣�ڻص�������ע�⴦�����������
	*   ����˵��
	*     WT_EXECUTEINPERSISTENTTHREAD  ���� Ҫ������̲߳�����ֹ����,ʹ�������̳߳ص��̣߳�Ӧ�ñ�֤������ĿѸ��ִ��(�ŵ���ʱ������߳���ִ��)
	*     WT_EXECUTELONGFUNCTION ���� ������Ŀ��Ҫ���Ѻܳ���ʱ�������У������̳߳ؾ����Ƿ�Ҫ�����߳����Ӹ��̳߳ء�
	*     WT_EXECUTEONLYONCE ����������Ŀִֻ��һ�ξ�ֹͣ
	*     WT_TRANSFER_IMPERSONATION
	* 
	*
    * ATL7��ʼ�ṩ������ɶ˿ڵ��̳߳�(CThreadPool)ģ����
    * Sample��http://msdn.microsoft.com/en-us/library/39f8fee2(VS.80).aspx
	*   ���ܵ�ʵ�֣�
	*     ����һ�� I/O ��ɶ˿ڣ�������һ���ȴ��ö˿ڵ��̳߳أ��򿪶�� I/O �豸�������ǵľ������ɶ˿ڹ���������
	*     ���첽 I/O �������ʱ���豸��������ͽ���������Ŀ���Ŷ��������ɶ˿ڡ�
    *   ȱ�㣺
    *     Ҫ����õ���������������ͬ��--��������������ֻ�ܰ���һģһ������������(���� QueueUserWorkItem ���)
    *     ���ܶ�̬�ı��С�����ܺܺõ���ͣ��ֹͣ ��
	* 
    * ʹ�÷�ʽ���μ� test_CFThreadPool
    *********************************************************************************************/
	//WaitForThreadpoolWorkCallbacks -- ����ȡ����δ���е����񣬵��޷�ȡ���Ѿ����е�����
	//http://www.cnblogs.com/wz19860913/articles/1274214.html --  Windows �̳߳�

	//Ŀǰ���̳߳� SubmitJob ��ʵ�ַ�ʽ���ܻ���� Job ������ʱ�����������û�з��ص�Bug ?
	//�����ڻص�����(��������÷��Ĺ�������) �� ���� SubmitJob �ĺ���(����÷��Ĺ��������м�����Ϣ) ͬ������Ԥ�����Bug

	//http://m.blog.csdn.net/blog/ithzhang/8373243

	//TODO:
	//  1.��������̸߳�����ʵ��(Ŀǰ���ô��������߳��������鷽ʽ���޷�֧�ֺܴ��ֵ)
	//  2.֧�����ú͵���Job������;
	//  3.֧�ֶ�̬�����̸߳���;
	//  4.֧�ֶ�̬����Job���ȼ�
	//  5.������Pool��ͣ��Ϊ֧�ֵ���Job��ͣ������װ����Pool��ͣ�ķ���;
	//  6.

	//! @brief ����ģ��������̳߳��࣬���ܿ��Է���Ľ��в��������⣬��ӵ�������ص㣺
	//!  1.���Զ�����������̵߳Ķ����� ��С/��� �̸߳���֮�����(Vista���ϵͳ�� SetThreadpoolThreadMaximum �Ⱥ��������ƹ���)
	//!  2.�ܷ���Ķ���һ�������ȡ������������δ�������ɿ�ܴ��봦�����������Ѿ����У�����Ҫ JobBase ��������� GetJobWaitType �ķ���ֵ���д���
	//!    (��Ƚ϶��ԣ�WaitForThreadpoolWorkCallbacks ����ֻ��ȡ����δ���е�����)
	//!  3.�ܶ������̳߳ؽ��� ��ͣ��������ֹͣ ���� -- ��Ҫ JobBase ��������� GetJobWaitType �ķ���ֵ���д���
	//!  4.֧�ֻص���ʽ�ķ���֪ͨ( Progress/Error ��)
	//!  5.ʹ��ģ�淽ʽʵ�֣��ܷ���Ľ��в�������
	//!  6.�ڼ�������ʱ�����������ȼ�(Ŀǰ�в�֧�ֶ�̬����)
	//!  7.ʹ�õ���΢���Ļ���API����֧��WinXP��Vista��Win7�ȸ��ֲ���ϵͳ(CreateThreadpoolWork ��ֻ����Vista�����ʹ��)
	//!  8.�����ж��ʵ����������п���(QueueUserWorkItem ���ǵ�ʵ��)
	//!  9.���õ��ǶԳ�ģʽ���̳߳�(û�й����߳�)�����гɱ���
	template <typename T> class CFThreadPool;  

	//enum FJobStatus
	//{
	//	jsWaiting,
	//	jsDoing,
	//	jsCancel,	//������������Ϊcancel, ���Job����֧��Cancel�Ļ��������� GetJobWaitType ���ú���������
	//	jsDone,
	//	jsError,	
	//};

	//! @brief Job�ĸ��࣬����Ϊ��ʵ����ϸ�Ĺ��ܣ��������� Run �� OnFinalize ����,��ʵ���������Pool�еȴ�����
	//! @todo �����ʵ�ֶ�����в�ͬ�������͵�Job����ģ����Ϊ DWORD_PTR �ȿ���ת��Ϊָ������ͼ���
	FTLEXPORT template <typename T>
	class CFJobBase
	{
		friend class CFThreadPool<T>;   //����Threadpool���� m_pThreadPool/m_nJobIndex ��ֵ
	public:
		//! @param bSuspendOnCreate [in] �����Ƿ񴴽������Job���������ڱ�֤��ʼ������(������ CFThreadPool::ResumeJob)ǰJob��������
		FTLINLINE CFJobBase(IN BOOL bSuspendOnCreate = FALSE);
		FTLINLINE CFJobBase(IN const T& rJobParam);
		FTLINLINE virtual ~CFJobBase();

		//bool operator < (const CFJobBase & other) const;

		//! @brief ����Job�����ȼ�������ԽС�����ȼ�Խ��(�ڵȴ�����������Խǰ��)
		FTLINLINE LONG GetPriority() const { return m_nJobPriority; }

		//! @brief ����Job�����ȼ�, ����ԽС�����ȼ�Խ��(�ڵȴ�����������Խǰ��)��ȱʡֵ�� 0
		//! @warning �ú��������� CFThreadPool::SubmitJob ֮ǰ���ã�
		//!          �����Ҫ�����Ѿ�����Pool��Job���ȼ�������ͨ�� CFThreadPool::SetJobPriority ����
		FTLINLINE LONG SetPriority(LONG nNewPriority);

		//! @brief ����Job��Ψһ��ʶ����ֵ����Pool����,Job���ɸ���
		FTLINLINE LONG GetJobIndex() const;

		//! @brief �������õĴ�����Ϣ -- ��Щ������Ϣ�����������õ�
		FTLINLINE DWORD GetErrorStatus() const;
		FTLINLINE LPCTSTR GetErrorInfo() const;

		//FTLINLINE FJobStatus GetJobStatus() const { return m_JobStatus; }

		//! @brief Job��ʹ�õĲ������˴�Ϊ�˼򻯣�ֱ�Ӳ��ù��б����ķ�ʽ
		T		m_JobParam;
	protected:
		//! @brief   ����������صĺ���������ʵ�ʵ�ҵ���߼�
		//! @warning �����Run��ͨ����Ҫѭ������ GetJobWaitType �������Job״̬����������֧�� ��ͣ��������ֹͣ �Ȳ���
		virtual BOOL Run() = 0;

		//! @brief ��Pool���ý�����Դ�ͷŵĴ��麯�������Job��new�����ģ�ͨ����Ҫ�ڸú����е��� delete this(������������������ڹ�������)
		//! @param [in] isWaiting ��ʾ�Ƿ����ڵȴ�״̬�½�����,
		//!   TRUE -- δ���е�Job(ֱ��ȡ�����̳߳�ֹͣ);
		//!   FALSE -- ���й���Job,���������н�����Ҳ���������й����б�ȡ��
		virtual VOID OnFinalize(IN BOOL isWaiting) = 0;
	protected:
		FTLINLINE void _SetErrorStatus(DWORD dwErrorStatus, LPCTSTR pszErrorInfo);
		FTLINLINE void _NotifyProgress(LONGLONG nCurPos, LONGLONG nTotalSize);
		FTLINLINE void _NotifyError();
		FTLINLINE void _NotifyError(DWORD dwError, LPCTSTR pszDescription);
		FTLINLINE void _NotifyCancel();

		//! @brief ͨ���ú�������ȡ�̳߳ص�״̬(Stop/Pause)���Լ�Job�Լ���Stop, �÷�ͬ CFThread:GetThreadWaitType,
		//! @param [in] dwMilliseconds ��ʱֵ�������֧����ͣ�������� INFINITE���粻��֧����ͣ(�����紫��)��������� 0
		FTLINLINE FTLThreadWaitType GetJobWaitType(DWORD dwMilliseconds = INFINITE) const;
	private:
		//����Ϊ˽�еı����ͷ�������ʹ������Ҳ��Ҫֱ�Ӹ��ģ���Pool���ý��п���
		BOOL		m_bSuspendOnCreate;
		LONG		m_nJobPriority;
		LONG		m_nJobIndex;						//! Job��Ψһ��ʶ��ֻ����Pool����
		DWORD				m_dwErrorStatus;
		CFStringFormater	m_strFormatErrorInfo;		
		HANDLE		m_hEventJobContinue;				//! Job����ʱ��Ҫ������¼�,������¼�Ϊ�Ǽ���״̬�������ʹJob��ͣ
		HANDLE		m_hEventJobStop;					//! ֹͣJob���¼����ñ�������Pool�������ͷ�(TODO:Pool�л���?)
		//FJobStatus	m_JobStatus;
		CFThreadPool<T>* m_pThreadPool;

		//��Pool���õ� ��ͣ��������ȡ���Ⱥ��� -- Job����ֱ�ӵ���(Job����Pool����ʱ���ܽ������û��˱�����ָ����ܻ��ΪҰָ��)
		FTLINLINE BOOL _Pause();
		FTLINLINE BOOL _Resume();
		FTLINLINE BOOL _Cancel();
	};


	//! �ȴ�Job�ıȽϺ��������ڱȽ�Job�Ĵ�С��ȷ���� Waiting �����е�˳�� ��������Ϊ Priority -> Index��
	struct WaitingJobSorter
	{
	public:
		FTLINLINE WaitingJobSorter(LONG nJobPriority, LONG nJobIndex)
		{
			this->nJobPriority = nJobPriority;
			this->nJobIndex = nJobIndex;
		}
		FTLINLINE WaitingJobSorter(const WaitingJobSorter&  rOther)
		{
			nJobPriority = rOther.nJobPriority;
			nJobIndex = rOther.nJobIndex;
		}
		FTLINLINE bool operator < (const WaitingJobSorter & other) const
		{
			COMPARE_MEM_LESS(nJobPriority, other);
			COMPARE_MEM_LESS(nJobIndex, other);
			return false;
		}
	private:
		LONG nJobPriority;
		LONG nJobIndex;
	};

	//! @brief �ص��ӿڣ�Ӧ���߼�����Լ̳иýӿڲ�������ط�������ȡPool���еĸ���״̬��Ϣ��
	//! ͨ�� pJob->m_JobParam ���Է�������Ϊ T �� ����
	FTLEXPORT template <typename T>
	class IFThreadPoolCallBack
	{
	public:
		//! ��Job���������Ժ󣬻��� Pool ���� Begin �� End ��������
		FTLINLINE virtual void OnJobBegin(LONG nJobIndex, CFJobBase<T>* pJob )
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobBegin[%d]\n"), this, nJobIndex);
		} 
		//! ��Job���������Ժ󣬻��� Pool ���� Begin �� End ��������
		FTLINLINE virtual void OnJobEnd(LONG nJobIndex, CFJobBase<T>* pJob)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobEnd[%d]\n"), this, nJobIndex);
		}

		//! �����δ��������״̬�ͱ�ȡ����Job������Pool�����������
		FTLINLINE virtual void OnJobCancel(LONG nJobIndex, CFJobBase<T>* pJob)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobCancel[%d]\n"), this, nJobIndex);
		}

		//! �� JobBase ������ͨ�� _NotifyProgress ����
		FTLINLINE virtual void OnJobProgress(LONG nJobIndex , CFJobBase<T>* pJob, LONGLONG nCurPos, LONGLONG nTotalSize)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			UNREFERENCED_PARAMETER(nCurPos);
			UNREFERENCED_PARAMETER(nTotalSize);
		}
		//! �� JobBase ������ͨ�� _NotifyError ����
		FTLINLINE virtual void OnJobError(LONG nJobIndex , CFJobBase<T>* pJob, DWORD dwError, LPCTSTR pszDescription)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			UNREFERENCED_PARAMETER(dwError);
			UNREFERENCED_PARAMETER(pszDescription);
			FTLTRACEEX(FTL::tlError, TEXT("IFThreadPoolCallBack[0x%x]::OnJobError[%d], dwError=%d, pszDesc=%s\n"), 
				this, nJobIndex, dwError, pszDescription);
		}
	};

	//! Pool�й����̻߳�ȡJobʱ�ĸ��ַ���ֵ
	enum GetJobType
	{
		typeStop,				//! ��ȡJobʱ����Pool��ֹͣ���߳̽��˳�
		typeSubtractThread,		//! ��Ҫ����һ���̣߳�����ֵΪ��ֵ���߳̽��˳�
		typeGetJob,				//! ���һ��Job������ֵΪ��ֵ���߳̽����л�õ�Job
		typeError,				//! ����δ֪���� -- Ŀǰ�в����ʲô����»ᷢ��(������ؾ����ǿ�ƹص�?)
	};

	FTLEXPORT template <typename T>  
	class CFThreadPool
	{
		//typedef CFSharePtr<CFJobBase< T> > CFJobBasePtr;
		friend class CFJobBase<T>;  //����Job�� GetJobWaitType �л�ȡ m_hEventStop
		DISABLE_COPY_AND_ASSIGNMENT(CFThreadPool);
	public:
		//! @brief Pool�Ĺ��캯��
		//! @param [in] pCallBack ״̬�ص���ָ�룬Ϊ�˼������ֻ���ڹ���������һ���������ú��ܸ���(��ҵ���������֤������)
		//! @param [in] nMaxWaitingJobs �ȴ�����������ܷ��ö���Job����������ֵ��Submit��������ʧ��ֱ����Job������
		FTLINLINE CFThreadPool(IFThreadPoolCallBack<T>* pCallBack = NULL, LONG nMaxWaitingJobs = LONG_MAX);

		FTLINLINE virtual ~CFThreadPool(void);

		//! ��ʼ�̳߳�,��ʱ�ᴴ�� nMinNumThreads ���̣߳�Ȼ�������������� nMinNumThreads �� nMaxNumThreads ֮�����е����̵߳ĸ���
		FTLINLINE BOOL Start(LONG nMinNumThreads, LONG nMaxNumThreads);

		//! �����̳߳����߳����ĵ��ڷ�Χ
		FTLINLINE BOOL GetThreadsCount(LONG* pMinNumThreads, LONG* pMaxNumThreads) const;

		//! ��̬�����̸߳������������С��0����ʾ���ı��Ӧ���߳���
		//! @note �����õ�Max�̸߳���С�ڵ�ǰ�Ѿ����е��̸߳���ʱ�������Զ�ֹͣ�����Job����Job�����󣬶�����߳̾ͻ��Զ��˳���
		FTLINLINE BOOL SetThreadsCount(LONG nMinNumThreads, LONG nMaxNumThreads);

		//! ����ֹͣ�̳߳�
		//! @note 1.ֻ������StopEvent����ҪJob�������GetJobWaitType����������������ֹͣ
		//! @note 2.���������ǰע��ĵ���δ���еĹ����������Ҫɾ������Ҫ����ClearUndoWork(���������л����)
		FTLINLINE BOOL Stop();

		//! ֹͣ���ȴ��̳߳ؽ���
		FTLINLINE BOOL StopAndWait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK);

		//! �ȴ������̶߳��������ͷ�Start�з�����߳���Դ
		FTLINLINE BOOL Wait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK);

		//! �����ǰδ���еĹ���
		FTLINLINE BOOL ClearUndoWork();
		
		//! ��ȡ��ǰ�̳߳ص�����״̬�����ص�ǰ�������е�Job�͵ȴ����е�Job����
		FTLINLINE BOOL GetRunningStatus(INT* pDoingJobCount, INT* pWaitingJobCount);

		//! @brief ���̳߳���ע�Ṥ�� -- �����ǰû�п��е��̣߳����ҵ�ǰ�߳���С������߳���������Զ������µ��̣߳�
		//!        �ɹ����ͨ�� pOutJobIndex ����Job�������ţ���ͨ����������λ�����ض���Job.
        //! @return ����TRUE��ʾ�ɹ�����Job������FALSE��ʾʧ�ܣ���Ҫͨ�� GetLastError ��ȡ��ϸ����Ϣ��
        //!   @retval ERROR_CANCELLED -- ����Jobʱ�̳߳�ֹͣ
        //!   @retval ERROR_TIMEOUT -- ��ʱ
		//! @warning �������FALSE�����÷���Ҫ�ͷ� pJob ��ص���Դ
		//! @todo ��������ʧ��ʱ�� Pool ���� pJob->OnFinalize(TRUE) ���ͷ���Դ�������ǵ��ظ��Լ��(��Ȼ�ù��������Ѿ�ɾ��)�����⣬���û����ɵ��÷��ͷűȽϺ���
		FTLINLINE BOOL SubmitJob(CFJobBase<T>* pJob, LONG* pOutJobIndex, DWORD dwMilliseconds = INFINITE);

		//! ��ָͣ����Job����ҪJob������ҵ���߼��������Ե��� GetJobWaitType ��֧��
		//! @note: ���ȡ��Job���ͻ������ܵ����ߵõ�ָ��ʱ��Jobִ����� delete this�������Ұָ���쳣
		FTLINLINE BOOL PauseJob(LONG nJobIndex);

		//! �ָ�ָ��Job����
		FTLINLINE BOOL ResumeJob(LONG nJobIndex);

		//! ȡ��ָ����Job,���Job��δ���У���ֱ�������Job������������У�������� _Cancel ���������Job�Ѿ�������ϣ���ֱ�ӷ���TRUE
		FTLINLINE BOOL CancelJob(LONG nJobIndex);

		//! ��ȡJob���ȼ�
		FTLINLINE BOOL GetJobPriority(LONG nJobIndex, LONG& nJobPriority);

		//! ��̬����Job�����ȼ�,���Job��δ���У��������ڵȴ������е�λ�ã����Job�������У��򵥸�������ֵ����ֵ
		FTLINLINE BOOL SetJobPriority(LONG nJobIndex, LONG nNewPriority);

		//! ������ͣ�̳߳صĲ���
		FTLINLINE BOOL PauseAll();

		//! ��������̳߳صĲ���
		FTLINLINE BOOL ResumeAll();

		//! �Ƿ��Ѿ�������ֹͣ�̳߳�
		FTLINLINE BOOL HadRequestStop() const;
	protected:
		//! �������е��߳�,��� ��ǰ�߳��� + nThreadNum <= m_nMaxNumThreads ʱ ��ɹ�ִ��
		FTLINLINE BOOL _AddJobThread(LONG nThreadNum);
		FTLINLINE void _DestroyPool();
		FTLINLINE void _DoJobs();

		FTLINLINE GetJobType _GetJob(CFJobBase<T>** ppJob);

		FTLINLINE void _NotifyJobBegin(CFJobBase<T>* pJob);
		FTLINLINE void _NotifyJobEnd(CFJobBase<T>* pJob);
		FTLINLINE void _NotifyJobCancel(CFJobBase<T>* pJob);

		FTLINLINE void _NotifyJobProgress(CFJobBase<T>* pJob, LONGLONG nCurPos, LONGLONG nTotalSize);
		FTLINLINE void _NotifyJobError(CFJobBase<T>* pJob, DWORD dwError, LPCTSTR pszDescription); 

		typedef BOOL (CALLBACK CFThreadPool::*HandleJobProc)(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL _FindAndHandleSpecialJob(LONG nJobIndex, HandleJobProc pProc, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerPauseJob(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerResumeJob(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerCancelJob(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerGetJobPriority(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerSetJobPriority(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
	protected:
		//! �̳߳������ٵ��̸߳���
		LONG m_nMinNumThreads;

		//! �̳߳��������̸߳���
		LONG m_nMaxNumThreads;

		//! �ȴ������е�����̸߳���
        LONG m_nMaxWaitingJobs;

		//! �ص��ӿ�
		IFThreadPoolCallBack<T>* m_pCallBack;

		//! Job��������ÿ SubmitJob һ�Σ������1������������¿�ʼ
		LONG m_nJobIndex;

		//! ��ǰ�������е�Job����
		LONG m_nRunningJobNumber;

		//! ��ǰ�����ŵ��̸߳���(���������е��߳̽���ʱ���� Complete �¼�)
		LONG m_nRunningThreadNum;

		typedef std::map<DWORD, HANDLE>   TaskThreadContrainer;
		//! ���������߳̾�������������� ThreadId -> Handle ��ӳ��
		TaskThreadContrainer m_TaskThreads;

		//! ����ȴ�Job����Ϣ�����������ȼ������⣬����һ���Ǵ���ǰ�濪ʼȡ����˱���ɰ��� WaitingJobSorter �����map
		typedef std::map<WaitingJobSorter, CFJobBase<T> * > WaitingJobContainer;
		typedef std::map<LONG, CFJobBase<T>* >	IndexToJobContainer;

		//! �ȴ����е�Job,���� WaitingJobSorter ����� map
		//! @see m_WaitingIndexJobs
		//! @note �����������汣���Jobָ����һ���ģ�ֻ�ǲ�ѯ��ʽ��һ��
		WaitingJobContainer		m_WaitingJobs;
		//! �ȴ����е�Job������ JobIndex ����� map
		//! @see m_WaitingJobs
		//! @note �����������汣���Jobָ����һ���ģ�ֻ�ǲ�ѯ��ʽ��һ��
		IndexToJobContainer		m_WaitingIndexJobs;

		//! �������е�Job�����ڻ�Ƶ�����롢ɾ��������Ҫ����JobIndex���ң���˱���ɰ� JobIndex ���ҵ� map
		IndexToJobContainer		m_DoingJobs;

		//! ֹͣPool���¼�
		HANDLE m_hEventStop;

		//! ���е��̶߳�����ʱ��������¼�
		//! @warning ���¼�����ʱֻ�������һ��Job�߳̽��˳��̳߳ص�Jobִ�к����������������˳�(����ҵ���߼���������߳��˳�ʱ����?),
		//!          ���ʱ�� Wait ������ͨ�����Է�ʽ��ʾ������Ա��������Ա��Ҫ���н����Щ���⡣
		HANDLE m_hEventAllThreadComplete;

		//! ����ȴ������л����Էŵ�Job������ÿȡ��һ��Job������1��ÿSubmitһ����ȥ�ͼ�1
        HANDLE m_hSemaphoreWaitingPos;

		//! ���滹�ж��ٸ�Job���ź���,ÿSubmitһ��Job,������һ��
		HANDLE m_hSemaphoreJobToDo;

		//! ���ڼ����̸߳���ʱ���ź���,��ʼʱ����Ϊ0,ÿҪ�ͷ�һ����������һ����
		HANDLE m_hSemaphoreSubtractThread;

		//! ���� m_DoingJobs ʱ����
		CFCriticalSection m_lockDoingJobs;

		//! ���� m_WaitingJobs ʱ����
		CFCriticalSection m_lockWaitingJobs;

		//! ���� m_pJobThreadHandles/m_pJobThreadIds ʱ����
		CFCriticalSection m_lockThreads;

		//! �����̵߳�ִ�к���
		static unsigned int CALLBACK JobThreadProc(void *pParam);
	};
}

#endif //FTL_THREADPOOL_H

#ifndef USE_EXPORT
#  include "ftlThreadPool.hpp"
#endif