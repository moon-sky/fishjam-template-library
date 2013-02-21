<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- Edited with XML Spy v4.2 -->
<html xsl:version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:variable name ="debug" select ="."/>
  <script language="JavaScript" src="trace.js"></script>
  <body style="font-family:Arial,helvetica,sans-serif;font-size:12pt;
        background-color:#EEEEEE" ondblclick="traceHTML()">
    <xsl:for-each select="breakfast_menu/food">
      <div id="div1" style="background-color:teal;color:white;padding:4px">
        <span style="font-weight:bold;color:white">
        <xsl:value-of select="name"/></span>
        - <xsl:value-of select="price"/>
      </div>
      <div style="margin-left:20px;margin-bottom:1em;font-size:10pt">
        <xsl:value-of select="description"/>
        <span style="font-style:italic">
          (<xsl:value-of select="calories"/> calories per serving)
        </span>
		<xsl:message terminate ="no">aaaaaaaaaaaaaa</xsl:message>
      </div>
    </xsl:for-each>
  </body>
</html>
