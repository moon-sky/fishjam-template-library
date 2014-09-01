package com.fishjam.utility.image.test;

import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Iterator;

import javax.imageio.IIOImage;
import javax.imageio.ImageIO;
import javax.imageio.ImageWriteParam;
import javax.imageio.ImageWriter;
import javax.imageio.stream.FileImageOutputStream;

import junit.framework.TestCase;

import org.junit.Test;

public class ImageTest  extends TestCase{
	
	final static String IMAGE_PNG_FILE_NAME = "testImageIo.png";
	final static String IMAGE_JPEG_FILE_NAME = "testImageIo.jpg";
	
	final static String IMAGE_OUTPUT_TYPE = "png";
	final static int     nWidth = 100, nHeight = 100;
	
	private BufferedImage generateImage(){
		//生成内存图片，并进行绘制
		BufferedImage outImage = new BufferedImage(nWidth, nHeight, BufferedImage.TYPE_INT_ARGB);	//TYPE_3BYTE_BGR
		
		//TODO:绘制方法可能有问题
		outImage.getGraphics().setColor(Color.RED);	
		outImage.getGraphics().fillRect(5, 5, 90, 90);
		//outImage.getGraphics().drawImage(img, dx1, dy1, dx2, dy2, sx1, sy1, sx2, sy2, observer)
		
		return outImage;
	}
	
	@Test
	public void testImageIo(){
		
		try {
			BufferedImage outImage = generateImage();
			
			//将图片内容写入文件
			File outFile = new File(IMAGE_PNG_FILE_NAME);
			ImageIO.write(outImage, IMAGE_OUTPUT_TYPE, outFile);
			
			//将图片内容转换成二进制数组
			ByteArrayOutputStream bout = new ByteArrayOutputStream(1024);  
            ImageIO.write(outImage, IMAGE_OUTPUT_TYPE, bout);  
            byte[] data = bout.toByteArray();  
            assertEquals(215, data.length );   //nWidth * nHeight * 4 -- 经过压缩的数据
			
			//从文件读取图片
			File inFile = new File(IMAGE_PNG_FILE_NAME);
			BufferedImage inImage = ImageIO.read(inFile);
		
			assertEquals(100, inImage.getWidth());
			assertEquals(100, inImage.getHeight());
			
			//该值始终为0 -- TODO: 有不为0的时候吗?
			assertEquals(0,  inImage.getMinX());	
			assertEquals(0,  inImage.getMinY());
			
			//获取某一点的像素值
			int nColor = inImage.getRGB(nWidth / 2 - 1,  nHeight / 2- 1);  //TODO: 设置的是 RED，为什么返回 -1(WHITE)
			assertEquals(-1, nColor);
			assertEquals(-1, Color.WHITE.getRGB());
			
			Color color = new Color(nColor);
			assertEquals(255, color.getAlpha());
			assertEquals(255, color.getRed());
			assertEquals(255, color.getGreen());
			assertEquals(255, color.getBlue());
			
			int[] rgba = new int[4];
			rgba[0] =(nColor & 0xff0000) >> 16;
			rgba[1] =(nColor & 0xff00) >> 8;
			rgba[1] =(nColor & 0xff);
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	@Test
	public void testSaveImageUsingJPGWithQuality(){
		//设置JPEG保存时的质量
		BufferedImage outImage = generateImage();
		float quality = 0.8F; 
		
		Iterator<ImageWriter> iter = ImageIO.getImageWritersByFormatName("jpeg");  
  
        ImageWriter imageWriter = iter.next();  
        ImageWriteParam iwp = imageWriter.getDefaultWriteParam();  
  
        iwp.setCompressionMode(ImageWriteParam.MODE_EXPLICIT);  
        iwp.setCompressionQuality(quality);  
  
        File file = new File(IMAGE_JPEG_FILE_NAME);  
        FileImageOutputStream fileImageOutput;
		try {
			fileImageOutput = new FileImageOutputStream(file);
	        imageWriter.setOutput(fileImageOutput);  
	        IIOImage iio_image = new IIOImage(outImage, null, null);  
	        imageWriter.write(null, iio_image, iwp);  
	        imageWriter.dispose();  
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}  

	}
}
