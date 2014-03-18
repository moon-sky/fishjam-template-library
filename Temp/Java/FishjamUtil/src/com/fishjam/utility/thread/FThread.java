package com.fishjam.utility.thread;

/*******************************************************************************
 * 还有很多功能没有实现和测试 -- 比如：二重启动的判断、Sleep的时间调整等
 * 
 * Java中实现多线程有两种途径：继承Thread类或者实现Runnable接口
 * 使用 synchronized 关键字同步，其关键是多个线程对象竞争同一个共享资源
 *   1.将函数声明为 synchronized,同步方法不会继承(父类是，子类不会自动是)
 *   2.使用特定变量，对指定代码块同步(关键区)
 * 
 * wait() 必须在synchronized 函数或者代码块里面,会让已经获得synchronized 函数或者
 *   代码块控制权的Thread暂时休息，并且自动释放synchronized锁，其他同步函数/block可被调用
 *     
 * 可能抛出 IllegalMonitorStateException 异常--只能对自己拥有锁的对象调用wait/notify 
 * 
 * daemon线程 -- 在背景提供通用性服务的线程，使用setDaemon切换。主线程结束后，所有的
 *   daemon线程会自动强制终止并结束程序。而如果有非daemon线程，则程序会等待其结束。
 * 
 * ThreadGroup -- 线程组，所有的线程都隶属于某个线程组，缺省为系统线程组
 ******************************************************************************/

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
