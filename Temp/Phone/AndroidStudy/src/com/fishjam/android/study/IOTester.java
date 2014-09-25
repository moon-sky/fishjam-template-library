package com.fishjam.android.study;
import android.test.AndroidTestCase;


/***************************************************************************************************************************************
 * OutputStream
 *   write
 * InputStream 
 * FileWriter 
 * 
 * String logdir  = Environment.getExternalStorageDirectory().getAbsolutePath()+ File.separator + "snda"+File.separator+"log" ; 
***************************************************************************************************************************************/

public class IOTester extends AndroidTestCase {

	public void testFileIO(){
		//读写文件的示例 -- TODO: 未实测和仔细考虑
		/*
		File fileInput = new File("inputFilePath");
		File fileOutput = new File("OutputFilePath");
		try {
			FileInputStream in = new FileInputStream(fileInput);
			FileOutputStream out = new FileOutputStream(fileOutput);
			ByteArrayOutputStream outBuffer = new ByteArrayOutputStream();
			int read = 0;
			while ((read = in.read()) != -1) {
				out.write(read);
			}
			in.close();
			out.flush();
			out.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		fileInput.length();
		*/
	}
}
