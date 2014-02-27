package com.fishjam.utility.net;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

/*******************************************************************************
* Java将socket包装成stream对象，使用I/O指令完成数据交互
* 默认情况下，socket的read/write会阻塞，必须改成非阻塞方式，才可用select等
* 
* Http是一种无session特性的通信协议，服务器无法区分获得的两次请求是否来自同一使用者，
*   目前已有很多方式追踪session：如 Cookie类、URL改写机制
*   Session对象存活于通信连接的服务端，目的是希望当客户端于网上浏览并与网络进行互动时，
*   能捕捉客户端相关数据，其数据只适用于该session－－如购物列表
*   
* JSP定义于servlet上的一个Java标准延伸机制，可将网页的HTML部分和多段Java程序代码合并
*   在单一文档中，当JSP容器首次装载JSP时，会自动产生并编译出符合JSP的servlet程序代码，
*   并加载至servlet容器。
*******************************************************************************/
public class FNet
{
    public void Server() throws IOException
    {
        ServerSocket ss = new ServerSocket(1000);
        Socket cs = ss.accept();
        BufferedReader in = new BufferedReader(
                new InputStreamReader(cs.getInputStream()));
        PrintWriter out = new PrintWriter(
                new BufferedWriter(
                        new OutputStreamWriter(
                                cs.getOutputStream())),
                        true); //println后自动发送网络包，否则需要等缓冲区满。注意数据交互和效率的平衡
        
        in.close();
        out.close();
        cs.close();
        ss.close();
    }
    
    public void Client() throws UnknownHostException, IOException
    {
        Socket s = new Socket("127.0.0.1",1000);
        //s.connect(endpoint);
        BufferedReader in = new BufferedReader(
                new InputStreamReader(s.getInputStream()));
        PrintWriter out = new PrintWriter(
                new BufferedWriter(
                        new OutputStreamWriter(
                                s.getOutputStream())),
                        true);
        
        in.close();
        out.close();
        s.close();
    }
}
