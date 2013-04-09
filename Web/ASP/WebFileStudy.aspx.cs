using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;

/************************************************************************************************************
 * IO相关的类
 *   Directory/DirectoryInfo
 *   DriveInfo -- 驱动器信息
 *   File/FileInfo
 *   Path -- 处理文件或目录路径的字符串
 * 
 * 读写文件：
 *   文本(StreamWriter/StreamReader) -- 可以直接创建或 File.CreateText/File.OpenText/File.Create 创建
 *     例: FileStream fs = File.Open(@"path.txt", FileMode.Open, FileAccess.Read, FileShare.Read);
 *         StreamReader reader = new StreamReader(fs);  //fileInfo.OpenText();
 *   二进制(BinaryWriter/BinaryReader) -- File.OpenWriter
 *     例: BinaryWriter writer = new BinaryWriter(File.OpenWriter(@"path.bin"));
 *     
************************************************************************************************************/

/************************************************************************************************************
 * 上传文件(FileUpload)
 *   在Upload按钮的代码中:
 *     string fullUploadPath = Path.Combine(uploadDirectory, Path.GetFileName(myFileUpload.PostedFile.FileName));
 *     myFileUpload.PostedFile.SaveAs(fullUploadPath);
 *  
 * 缺省时，ASP.NET 拒绝超过4M的文件上传请求，可以修改 web.config 中的 maxRequestLength(单位为K) 来修改限制，如 ="10240" 表示10M.
************************************************************************************************************/

public partial class WebFileStudy : System.Web.UI.Page
{
    string curDirectory;
    protected void Page_Load(object sender, EventArgs e)
    {
        //当前应用程序目录路径
        curDirectory = Request.PhysicalApplicationPath; //Path.Combine(, "Uploads");
        //curDirectory = Server.MapPath("Uploads");

        if (!this.IsPostBack)
        {
            CreateFileList();
        }
    }

    private void CreateFileList()
    {
        string[] files = Directory.GetFiles(curDirectory);
        ListBox1.DataSource = files;
        ListBox1.DataBind();
    }
}
