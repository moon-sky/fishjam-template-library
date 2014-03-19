package com.storeserver.servlet;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.annotation.MultipartConfig;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.Part;

@WebServlet(name = "upload", urlPatterns = { "/upload" } )
@MultipartConfig
public class UploadServlet extends HttpServlet {

	@Override
	protected void service(HttpServletRequest request, HttpServletResponse response)
			throws ServletException, IOException {
		// TODO Auto-generated method stub
		response.setContentType("text/html;charset=UTF-8");
		
		PrintWriter out=response.getWriter();
		String fileName=request.getParameter("name");
		
		Part part=request.getPart("map");
		out.println("上传文件的类型："+part.getContentType()+"<br/>");
		out.println("上传文件的大小"+part.getSize()+"<br/>");
		
	}
}
