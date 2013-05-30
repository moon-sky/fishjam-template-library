#pragma once

class CUserInfoManager : public FTL::CFSingletonT<CUserInfoManager>
{
public:
	CUserInfoManager(void);
	~CUserInfoManager(void);

public:
	const CString& GetUserName() const { return m_strUserName; }
	void SetUserName(CString val) { m_strUserName = val; }
	LONG GetCurrentMoney() const { return m_nCurrentMoney; }
	void SetCurrentMoney(LONG val) { m_nCurrentMoney = val; }

private:
	CString		m_strUserName;				//用户名
	LONG		m_nCurrentMoney;			//当前的金钱数
};
