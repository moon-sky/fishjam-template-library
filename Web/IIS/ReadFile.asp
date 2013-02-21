<%@ LANGUAGE="VBSCRIPT %>
<%
  Server.ScriptTimeout = 36000
  PostSize = Request.TotalBytes
  'Read POST data in 1K chunks
  
  BytesRead = 0
  For i = 1 to (PostSize/1024)
    ReadSize = 1024
    PostData = Request.BinaryRead(ReadSize)
    BytesRead = BytesRead + ReadSize
  Next

  'Read remaining fraction of 1K
  ReadSize = TotalBytes - BytesRead
  If ReadSize <> 0 Then
    PostData = Request.BinaryRead(ReadSize)
    BytesRead = BytesRead + ReadSize
  End IF

  'Send results back to client
  Response.Write BytesRead
  Response.Write " bytes were read."  
 %>