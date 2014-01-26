package com.fishjam.util;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.lang.Thread.UncaughtExceptionHandler; 

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.os.Looper;

/**************************************************************************************************************************************
 * http://www.cnblogs.com/freeliver54/archive/2011/10/25/2223729.html
 * 
 * 应用程序全局未捕获全局异常的处理
 *   通过Thread.setDefaultUncaughtExceptionHandler()方法将异常处理类设置到线程
 *   
***************************************************************************************************************************************/


public class CrashHandler implements UncaughtExceptionHandler {
	private static final String TAG = CrashHandler.class.getSimpleName();
	private static CrashHandler instance;
    private Context mContext;  
    private Thread.UncaughtExceptionHandler mDefaultHandler;  
    //private Throwable mException;
    
    private CrashHandler() {  
    }  
  
    public static CrashHandler getInstance() {  
    	if (instance == null) {
    		instance = new CrashHandler(); 
		}
        return instance;  
    }  
    
    public void init(Context ctx) {  
        mContext = ctx;  
        mDefaultHandler = Thread.getDefaultUncaughtExceptionHandler();  
        Thread.setDefaultUncaughtExceptionHandler(this);  
    }  
    
    public static String FormatStackTrace(Throwable throwable) {  
        if(throwable==null) return "";  
        String rtn = throwable.getStackTrace().toString();  
        try {  
        	StringWriter stringWriter = new StringWriter();  
            PrintWriter printWriter = new PrintWriter(stringWriter);  
            throwable.printStackTrace(printWriter);       
            printWriter.flush();  
            stringWriter.flush();  
            rtn = stringWriter.toString();  
            printWriter.close();              
            stringWriter.close();  
        } catch (IOException e) {  
            e.printStackTrace();  
        } catch (Exception ex) {  
        }  
        return rtn;  
    }  
    
	@Override
	public void uncaughtException(Thread thread, final Throwable ex) {
		System.out.println("uncaughtException");
		ex.printStackTrace();
		//mException = ex;
		
		new Thread() {  
            @Override  
            public void run() {  
                Looper.prepare();  
                String strMsg = FormatStackTrace(ex);

                new AlertDialog.Builder(mContext).setMessage(strMsg).setTitle("提示").setCancelable(false)  
                        .setPositiveButton("Close", new OnClickListener() {
                        	@Override
							public void onClick(DialogInterface dialog, int which) {
                        		System.exit(0);
							}  
                        }).create().show();  
                Looper.loop();  
            }  
        }.start();  
        
        //ex.printStackTrace();  
        //android.os.Process.killProcess(android.os.Process.myPid());   
	}

}
