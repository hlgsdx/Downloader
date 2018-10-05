#pragma once
#include <list>
#include <queue>
#include <Windows.h>
#include <process.h>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
class MyLock {
public:
	MyLock() {
		InitializeCriticalSection(&m_cs);
	}
	~MyLock() {
		DeleteCriticalSection(&m_cs);
	}
	void Lock() {
		EnterCriticalSection(&m_cs);
	}
	void UnLock() {
		LeaveCriticalSection(&m_cs);
	}
private:
	CRITICAL_SECTION m_cs;
};
class Itask {
public:
	Itask() {}
	virtual ~Itask() {}
public:
	virtual void RunItask() = 0;
};


class CMyThreadPool {
public:
	CMyThreadPool(void);
	~CMyThreadPool(void);
public:
	//1.创建一些线程
	bool CreateThreadPool(long lMinThreadNum, long lMaxThreadNum);
	//2.销毁线程池
	void DestroyThreadPool();
	//3.线程函数
	static   unsigned _stdcall ThreadProc( void * );

	//4.投递任务
	bool Push(Itask*);
private:
	list<Itask*> m_qItask;
	list<HANDLE>  m_lstThread;
	bool  m_bFlagQuit;
	HANDLE m_hSemaphore;
	long   m_lCreateThreadNum; //创建线程的个数
	volatile	long   m_lRunThreadNum; //运行的线程个数
	long   m_lMaxThreadNum;//线程的最大值
	MyLock m_lock;
};

