package com.storeserver.action;
import java.io.File;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.Part;

import org.apache.struts2.interceptor.ServletRequestAware;

import com.fishjam.utility.net.servlet.ServletInfoDumper;
import com.storeserver.action.base.BaseAction;
import com.storeserver.service.StoreInfoManager;

//@MultipartConfig
public class AddStoreMapAction extends BaseAction implements ServletRequestAware {

 private HttpServletRequest mRequest;
 
@Override
 	public void setServletRequest(HttpServletRequest request) {
 		mRequest = request;
 	}

	public String execute() {
		//mRequest.getParameter("")
		System.out.println("AddStoreMapAction:\n" + ServletInfoDumper.RequestToString(mRequest, "\n"));
		
		StoreInfoManager.FloorInfo floorInfo = new StoreInfoManager.FloorInfo();
		try {
			floorInfo.floor = Integer.parseInt(mRequest.getParameter("floor"));
			floorInfo.storeId = Integer.parseInt(mRequest.getParameter("storeId"));

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
			//mManager.insertFloorInfo(this, "hqzx.db", floorInfo);
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