<%@ Page Language="C#" AutoEventWireup="true" CodeFile="WebSecurity.aspx.cs" Inherits="WebSecurity" MasterPageFile="~/MasterPage.master" %>

<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" runat="Server">
    <asp:GridView ID="GridView1" runat="server">
    </asp:GridView>
    <br />
            <asp:Label ID="lblSession" runat="server" Text=""></asp:Label>
    <br />
</asp:Content>
