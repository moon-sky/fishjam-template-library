using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Drawing;
using System.IO;

/********************************************************************************************************************************************
 * 通过GDI+动态绘制图形，绘制的图形可以直接呈现为HTML流，并直接发送给客户端浏览器 -- 注意：Chrome不能显示返回的gif图像，IE能显示
 * 
 *   1.创建Bitmap;
 *   2.创建Graphics;
 *   3.绘图
 *   4.绘制结束后，Image.Save 将所绘制的图形发送回浏览器的HTML输出流(Response.OutputStream)中,这样会覆盖所有的默认输出，
 *     可以通过Image指定空的 .aspx 来解决该问题。也可以保存到 FileStream 文件流中，这样可以动态生成图形，并使用在Web页面中。
********************************************************************************************************************************************/

public partial class WebGraphStudy : System.Web.UI.Page
{
    protected void DrawGdiPlusImage()
    {
        Bitmap bmp = null;
        Graphics g = null;
        try
        {
            bmp = new Bitmap(640, 480);
            g = Graphics.FromImage(bmp);
            
            g.FillRectangle(Brushes.Red, 80, 60, 380, 260);
            g.DrawRectangle(Pens.Blue, 80, 60, 380, 260);

            Font font = new Font("Arial", 20, FontStyle.Regular);
            {
                g.DrawString("Web Graph Study", font, Brushes.Green, 10, 10);
            }

            //通过 MemoryStream 来支持 PNG
            Response.ContentType = "image/png";
            MemoryStream ms = new MemoryStream();
            bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Png);

            //如果打开这句话，会在客户端输出图片，但会覆盖所有其他的HTML结果。 能插入吗？
            //ms.WriteTo(Response.OutputStream);
        }
        finally
        {
            //显式释放图形对象和Graphics图形表面 对应的非托管资源 -- TODO: 使用 using ?
            if (g != null)
            {
                g.Dispose();
            }
            if (bmp != null)
            {
                bmp.Dispose();
            }
        }


    }
    protected void Page_Load(object sender, EventArgs e)
    {
        DrawGdiPlusImage();
    }
}
