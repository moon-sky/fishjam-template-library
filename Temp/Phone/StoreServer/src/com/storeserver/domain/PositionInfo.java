package com.storeserver.domain;

import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.BufferedImageFilter;
import java.io.ByteArrayInputStream;
import java.io.File;

import javax.imageio.ImageIO;
import javax.imageio.stream.FileImageOutputStream;
import javax.imageio.stream.MemoryCacheImageInputStream;
import javax.servlet.ServletContext;

import org.apache.struts2.ServletActionContext;

import com.fishjam.utility.net.servlet.ServletInfoDumper;

public class PositionInfo {
	private StoreMapInfo mStoreMapInfo;
	private int mId;
	private int mMapId;
	private int mX;
	private int mY;
	private String mDesc;
	
	public StoreMapInfo getStoreMapInfo() {
		return mStoreMapInfo;
	}
	public void setStoreMapInfo(StoreMapInfo storeMapInfo) {
		this.mStoreMapInfo = storeMapInfo;
	}
	public int getId() {
		return mId;
	}
	public void setId(int id) {
		this.mId = id;
	}
	public int getX() {
		return mX;
	}
	public void setX(int x) {
		this.mX = x;
	}
	public int getY() {
		return mY;
	}
	public void setY(int y) {
		this.mY = y;
	}
	public String getDesc() {
		return mDesc;
	}
	public void setDesc(String desc) {
		this.mDesc = desc;
	}
	public void setMapId(int mapId) {
		this.mMapId = mapId;
	}
	public int getMapId(){
		return mMapId;
	}
	
	public String getPositionMapImage(){
		if (mStoreMapInfo != null) {
			//BufferedImage image = ImageIO.
			
		}
			
		return "";
	}

	@Override
	public String toString() {
		StringBuilder sBuilder = new StringBuilder();
		sBuilder.append("id=" + getId() + ", X=" + getX() +",Y=" + getY() + ",Desc=" + getDesc());
		sBuilder.append(", mapInfo=" + getStoreMapInfo().toString());
		return sBuilder.toString();
	}
}
