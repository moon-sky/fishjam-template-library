package com.storeserver.action;
import java.io.File;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.Part;

import org.apache.commons.io.FileUtils;
import org.apache.struts2.ServletActionContext;
import org.apache.struts2.interceptor.ServletRequestAware;
import org.aspectj.util.FileUtil;

import com.fishjam.utility.dumper.BaseInfoDumper;
import com.fishjam.utility.net.servlet.ServletInfoDumper;
import com.opensymphony.xwork2.ActionContext;
import com.storeserver.action.base.BaseAction;
import com.storeserver.service.StoreInfoManager;

//@MultipartConfig
public class AddStoreMapAction extends BaseAction implements ServletRequestAware {

	 private HttpServletRequest mRequest;
	 
	 private File mMapFile;
	 public File getMapFile() {
		return mMapFile;
	}

	public void setMapFile(File mapFile) {
		this.mMapFile = mapFile;
	}

	public File getPathFile() {
		return mPathFile;
	}

	public void setPathFile(File pathFile) {
		this.mPathFile = pathFile;
	}

//	private String mMapFileName; //文件名称
//	 private String mMapFileContentType; //文件类型
//	 
	 private File mPathFile;
//	 private String mPathFileName; //文件名称
//	 private String mPathFileContentType; //文件类型

	 @Override
 	public void setServletRequest(HttpServletRequest request) {
 		mRequest = request;
 	}

	public String execute() {
		//mRequest.getParameter("")
		System.out.println("AddStoreMapAction:\n" + ServletInfoDumper.RequestToString(mRequest, "\n"));
		//System.out.println("AddStoreMapAction param:\n" + BaseInfoDumper.MapToString(ActionContext.getContext().getContextMap()));
		
		StoreInfoManager.FloorInfo floorInfo = new StoreInfoManager.FloorInfo();
		try {
			//if (mMapFile != null) {
				System.out.println("mapFile=" + mMapFile.getAbsolutePath());
				System.out.println("pathFile=" + mPathFile.getAbsolutePath());
				
				String realpath = ServletActionContext.getServletContext().getRealPath("/uploadTest");
				
				String strMapFile = mRequest.getParameter("map");
				if (strMapFile == null) {
					strMapFile = "mapFile";
				}
				
				String strPathFile = mRequest.getParameter("path");
				if (strPathFile == null) {
					strPathFile = "pathFile.map";
				}
				
				File mapSavefile = new File(new File(realpath), strMapFile);
	            if (!mapSavefile.getParentFile().exists())
	            	mapSavefile.getParentFile().mkdirs();
	            FileUtils.copyFile(mMapFile, mapSavefile);
	            
	            File pathSaveFile = new File(new File(realpath), strPathFile);
	            FileUtils.copyFile(mPathFile, pathSaveFile);
	            
	            floorInfo.mapDataStream = FileUtils.openInputStream(mapSavefile);
	            floorInfo.pathDataStream = FileUtils.openInputStream(pathSaveFile);
	            //ActionContext.getContext().put("message", "文件上传成功");
			//}
			
			floorInfo.floor = Integer.parseInt(mRequest.getParameter("StoreMapInfo.floor"));
			floorInfo.storeId = Integer.parseInt(mRequest.getParameter("StoreMapInfo.storeId"));

			/*
			Part partMap = mRequest.getPart("map");
			if (partMap != null) {
				floorInfo.mapDataStream = partMap.getInputStream();
				System.out.println("partMap=" + partMap.getSize());
			}
			Part partPath = mRequest.getPart("path");
			if (partPath != null) {
				floorInfo.pathDataStream = partPath.getInputStream();
				System.out.println("partPath=" + partPath.getSize());
			}
			*/
			
			mManager.insertFloorInfo(this, "hqzx.db", floorInfo);
			if (floorInfo.mapDataStream != null) {
				floorInfo.mapDataStream.close();
			}
			if (floorInfo.pathDataStream != null) {
				floorInfo.pathDataStream.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return SUCCESS;
	}
}