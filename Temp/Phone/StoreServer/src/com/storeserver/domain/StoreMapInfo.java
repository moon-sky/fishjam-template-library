package com.storeserver.domain;

import java.awt.image.BufferedImage;
import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.imageio.stream.FileImageOutputStream;
import javax.imageio.stream.MemoryCacheImageInputStream;
import javax.servlet.ServletContext;

import org.apache.struts2.ServletActionContext;

import com.fishjam.utility.net.servlet.ServletInfoDumper;

public class StoreMapInfo {
	private int mId;
	private int mStoreId;
	private int mFloor;
	
	private byte[] mPath;
	private BufferedImage mMapImage;
	private int mPathSize;
	private String mImagePath = "";
	
	public int getId() {
		return mId;
	}
	public void setId(int mId) {
		this.mId = mId;
	}
	public int getStoreId() {
		return mStoreId;
	}
	public void setStoreId(int mStoreId) {
		this.mStoreId = mStoreId;
	}
	public int getFloor() {
		return mFloor;
	}
	public void setFloor(int mfloor) {
		this.mFloor = mfloor;
	}

	public void setMap(byte[] map) {
		
		ByteArrayInputStream byteStream = new ByteArrayInputStream(map);
		MemoryCacheImageInputStream memoryStream = new MemoryCacheImageInputStream(byteStream);
		try {
			this.mMapImage = ImageIO.read(memoryStream);
			ServletContext servletContext = ServletActionContext.getServletContext();
			System.out.println("ServletContextInfo = \n" + ServletInfoDumper.ServletContextToString(servletContext, "\n"));
			
			String strImagePath = "/images/pos/" + getStoreId() + "/";
			String strParentPath =servletContext.getRealPath(strImagePath) + "/";
			File path = new File(strParentPath);
			if (!path.exists()){
				path.mkdirs();
			}
			String strFileName = "storeMap_" + getFloor() + ".jpg";
			mImagePath = servletContext.getContextPath() + strImagePath +  strFileName;
			
			File file = new File( strParentPath + strFileName);
			FileImageOutputStream outputStream = new FileImageOutputStream(file);
			
			outputStream.write(map);
			outputStream.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public byte[] getPath() {
		return mPath;
	}
	public void setPath(byte[] path) {
		this.mPath = path;
		this.mPathSize = path.length;
	}
	public int getPathSize()
	{
		return mPathSize;
	}
	
	public BufferedImage getMapImage() {
		return mMapImage;
	}
	public String getImagePath(){
		return mImagePath;
	}
	
	@Override
	public String toString() {
		StringBuilder sBuilder = new StringBuilder();
		sBuilder.append("id=" + mId + ",mImagePath=" + mImagePath + ",StoreId="+mStoreId +",Floor=" + mFloor);
		
		return sBuilder.toString();				
	}
}
