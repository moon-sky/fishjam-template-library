package com.storeserver.action;

import java.awt.Image;

import javax.servlet.http.HttpServletRequest;

import org.apache.struts2.interceptor.ServletRequestAware;

import com.fishjam.utility.net.servlet.ServletInfoDumper;
import com.storeserver.action.base.BaseAction;
import com.storeserver.domain.PositionInfo;
import com.storeserver.domain.StoreMapInfo;

//访问方式为 : http://localhost:8888/StoreServer/position?pos=storeId-PosId  如 1-123
public class DetectPosition extends BaseAction implements ServletRequestAware  {

	private HttpServletRequest mRequest;
	private PositionInfo mPositionInfo;
	private Image m_PositionImage;
	
	@Override
	public void setServletRequest(HttpServletRequest request) {
		this.mRequest = request;
	}

	public PositionInfo getPositionInfo() {
		return mPositionInfo;
	}
	
	public String execute() {
		System.out.println("DetectPosition: \n" + ServletInfoDumper.RequestToString(mRequest, "\n"));
		String strPosition = mRequest.getParameter("pos");
		if (strPosition != null) {
			mPositionInfo = mManager.getPositionInfo(this, strPosition);
			if (mPositionInfo != null) {
				//mPositionInfo.getStoreMapInfo().getMapImage();
				return SUCCESS;
			}
		}
		return INPUT;
	}

}