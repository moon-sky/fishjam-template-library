package com.fishjam.utility.thread;


/*********************************************************************************************************************
* 还有很多功能没有实现和测试 -- 比如：二重启动的判断、Sleep的时间调整等
*********************************************************************************************************************/
//Java线程相关的方法通过抛出 InterruptedException 异常来进行流程控制--设计差？

public abstract class FThread implements Runnable
{
    public enum ThreadWaitType
    {
    	
        twtContinue, twtTimeOut, twtStop, twtError
    };

    public FThread()
    {
        this(new Object());
    }

    public FThread(Object syncObject)
    {
        m_syncObject = syncObject;
        m_stop = false;
        m_continue = true;
    }

    public void start()
    {
        m_stop = false;
        m_continue = true;
        m_thread = new Thread(this);
        m_thread.start();
    }

    public void pause()
    {
        //是否需要同步？因为不需要唤醒，所以不需要？
        System.out.println("pause");
        m_continue = false;
    }

    public void resume()
    {
        synchronized (m_syncObject)
        {
            System.out.println("resume");
            m_continue = true;
            m_syncObject.notifyAll();
        }
    }

    public void stop()
    {
        synchronized (m_syncObject)
        {
            System.out.println("stop");
            m_stop = true;
            m_syncObject.notifyAll();
        }
    }

    public void join()
    {
        try
        {
            //Thread t = m_thread;
            //m_thread = null;
            //t.join();
            
            //没有使用同步，使用上一种方式可能出错？
            m_thread.join();
            m_thread = null;
        } catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }
    
    public ThreadWaitType getWaitType()
    {
        ThreadWaitType waitType = ThreadWaitType.twtContinue;
        try
        {   
            //避免频繁锁 -- 仔细考虑 m_stop？
            if(/*!m_stop && */!m_continue)
            {
                synchronized(m_syncObject)
                {
                    while (!m_stop && !m_continue)
                    {
                        m_syncObject.wait();
                    }
                }
            }
            if (m_stop)
            {
                waitType = ThreadWaitType.twtStop;
            }
        } catch (InterruptedException e)
        {
            //什么时候会发生？
            waitType = ThreadWaitType.twtError;
            e.printStackTrace();
        }
        return waitType;
    }

    public ThreadWaitType sleepAndCheckStop(long timeout)
    {
        ThreadWaitType waitType = ThreadWaitType.twtContinue;
        synchronized (m_syncObject)
        {
            try
            {
                m_syncObject.wait(timeout);
            } catch (InterruptedException e)
            {
                e.printStackTrace();
            }
        }
        return waitType;
    }
    
    private volatile boolean m_stop;
    private volatile boolean m_continue;
    private Thread m_thread;
    private Object m_syncObject;
}
