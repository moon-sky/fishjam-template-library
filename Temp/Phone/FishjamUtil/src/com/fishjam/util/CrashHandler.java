package com.fishjam.util;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.RandomAccessFile;
import java.io.StringWriter;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.LinearLayout.LayoutParams;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

/**************************************************************************************************************************************
 * http://www.cnblogs.com/freeliver54/archive/2011/10/25/2223729.html
 * http://stackoverflow.com/questions/5519347/android-uncaughtexceptionhandler-that-instantiates-an-alertdialog-breaks
 * http://stackoverflow.com/questions/13416879/show-a-dialog-in-thread-setdefaultuncaughtexceptionhandler
 * 
 * 应用程序全局未捕获全局异常的处理
 *   通过Thread.setDefaultUncaughtExceptionHandler()方法将异常处理类设置到线程
 *   
***************************************************************************************************************************************/

public class CrashHandler implements Thread.UncaughtExceptionHandler {
	
	@SuppressWarnings("unused")
	private static final String TAG = CrashHandler.class.getSimpleName();
	private static CrashHandler instance;
    private Context mContext;  
    private Thread.UncaughtExceptionHandler mDefaultHandler;  
    private boolean mWriteToLogs = true;
    
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

    public void testShowCrashActivity(Context ctx){
    	String strMsg = "some very long info\n another info";
    	Intent crashedIntent = new Intent(ctx, CrashInfoActivity.class);
        crashedIntent.putExtra(CrashInfoActivity.CRASH_INFO_STRING, strMsg);
        crashedIntent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET);
        crashedIntent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        ctx.startActivity(crashedIntent);
        
        System.exit(0);
    }
    
    public void setWriteToLogs(boolean writeToLogs) {
		this.mWriteToLogs = writeToLogs;
	}
    
    public static String FormatStackTrace(Throwable throwable) {  
        if(throwable==null) return "";  
        String rtn = throwable.getStackTrace().toString();  
        try {  
        	StringWriter localStringWriter = new StringWriter();  
            PrintWriter localPrintWriter = new PrintWriter(localStringWriter);  
            throwable.printStackTrace(localPrintWriter);
            
            localPrintWriter.flush();  
            localStringWriter.flush();  
            rtn = localStringWriter.toString();
            
            localPrintWriter.close();              
            localStringWriter.close();  
        } catch (Exception e) {
        	e.printStackTrace();
        }  
        return rtn;  
    }  
    
	@Override
	public void uncaughtException(Thread thread, final Throwable ex) {
		System.out.println("uncaughtException");
		ex.printStackTrace();

		String strMsg = FormatStackTrace(ex);
		if (mWriteToLogs) {
			writeStackTraceToFile(strMsg);
		}
		Toast.makeText(mContext, "StoreHelperCrash", Toast.LENGTH_LONG).show();
 
        android.os.Process.killProcess(android.os.Process.myPid());   
		
		/*
		Intent crashedIntent = new Intent(mContext, CrashInfoActivity.class);
        crashedIntent.putExtra(CrashInfoActivity.CRASH_INFO_STRING, strMsg);
        crashedIntent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET);
        crashedIntent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        mContext.startActivity(crashedIntent);
        
        new AlertDialog.Builder(mContext).setMessage(strMsg).setTitle("提示").setCancelable(false)  
                .setPositiveButton("Close", new OnClickListener() {
                	@Override
					public void onClick(DialogInterface dialog, int which) {
                		System.exit(0);
					}  
                }).create().show();  
        */
		
		/*new Thread() {  
            @Override  
            public void run() {  
                //Looper.prepare();  
                String strMsg = FormatStackTrace(ex);

                new AlertDialog.Builder(mContext).setMessage(strMsg).setTitle("提示").setCancelable(false)  
                        .setPositiveButton("Close", new OnClickListener() {
                        	@Override
							public void onClick(DialogInterface dialog, int which) {
                        		System.exit(0);
							}  
                        }).create().show();  
                //Looper.loop();  
            }  
        }.start();  */
        
        //ex.printStackTrace();  
        //android.os.Process.killProcess(android.os.Process.myPid());   
	}

	private void writeStackTraceToFile(String strMsg) {
		String baseDir = Environment.getExternalStorageDirectory().getAbsolutePath();
		File file = new File(baseDir + File.separator + "storeHelper_crash.log");

		try {
			if (!file.exists()) {
				file.createNewFile();
			}
			RandomAccessFile raf = new RandomAccessFile(file, "rw");
			raf.write(strMsg.getBytes());
			raf.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}


	
}
