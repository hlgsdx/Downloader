#include "stdafx.h"
#include "MyThreadPool.h"

CMyThreadPool::CMyThreadPool(void) {
    m_bFlagQuit = true;
    m_hSemaphore = NULL;
    m_lCreateThreadNum = 0;
    m_lRunThreadNum = 0;
    m_lMaxThreadNum = 0;
}


CMyThreadPool::~CMyThreadPool(void) {
}
bool CMyThreadPool::CreateThreadPool(long lMinThreadNum, long lMaxThreadNum) {
    if(lMinThreadNum <= 0 || lMaxThreadNum < lMinThreadNum)
        return false;

    //�����߳�����Ҫ����Դ
    m_hSemaphore = CreateSemaphore(NULL, 0, lMaxThreadNum, NULL);

    //1.�����߳�
    for(long i = 0; i < lMinThreadNum; i++) {
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &CMyThreadPool::ThreadProc, this, 0, 0);
        if(hThread)
            m_lstThread.push_back(hThread);
    }
    m_lCreateThreadNum = lMinThreadNum;
    m_lMaxThreadNum = lMaxThreadNum;
    return true;
}

void CMyThreadPool::DestroyThreadPool() {
    m_bFlagQuit = false;

    auto ite = m_lstThread.begin();
    while(ite != m_lstThread.end()) {
        if(WAIT_TIMEOUT == WaitForSingleObject(*ite, 100))
            TerminateThread(*ite, -1);

        CloseHandle(*ite);
        *ite = NULL;

        ++ite;
    }

    m_lstThread.clear();

    while(m_qItask.size() > 0) {

        //�Ӷ�����ȡ������
        Itask*	pItask = m_qItask.front();
        m_qItask.pop_front();

        delete pItask;
        pItask = NULL;
    }
    m_qItask.clear();
}

bool CMyThreadPool::Push(Itask *pItask) {
    if(pItask == NULL)return false;
    //��������뵽����
    m_lock.Lock();
    m_qItask.push_back(pItask);
    m_lock.UnLock();
    //�ͷ��ź���
    //1.�п����߳�
    if(m_lRunThreadNum < m_lCreateThreadNum) {
        ReleaseSemaphore(m_hSemaphore, 1, NULL);
    } else if(m_lCreateThreadNum < m_lMaxThreadNum) { //2.û���̣߳�����û�������̵߳ĸ���
        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, &CMyThreadPool::ThreadProc, this, 0, 0);
        if(hThread)
            m_lstThread.push_back(hThread);
        m_lCreateThreadNum++;
        ReleaseSemaphore(m_hSemaphore, 1, NULL);
    } else {
        //3.����

    }
    return true;
}

unsigned  __stdcall CMyThreadPool::ThreadProc( void *lpvoid ) {
    CMyThreadPool *pthis = (CMyThreadPool*)lpvoid;
    Itask *pItask = NULL;
    while(pthis->m_bFlagQuit) {
        //���ź���
        if(WAIT_TIMEOUT == WaitForSingleObject(pthis->m_hSemaphore, 100))
            continue;

        pthis->m_lRunThreadNum++;


        //�ж϶����Ƿ�Ϊ��
        while(pthis->m_qItask.size() > 0) {
            pthis->m_lock.Lock();
            if(pthis->m_qItask.size() == 0) {
                pthis->m_lock.UnLock();
                break;
            } else {
                //�Ӷ�����ȡ������
                pItask = pthis->m_qItask.front();
                pthis->m_qItask.pop_front();
            }

            pthis->m_lock.UnLock();

            //ִ������
            pItask->RunItask();

            delete pItask;
            pItask = NULL;
        }

        pthis->m_lRunThreadNum--;

    }

    return 0;
}