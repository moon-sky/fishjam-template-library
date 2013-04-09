<%@ Page Title="" Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="DataBindingStudy.aspx.cs" Inherits="DataBindingStudy" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" Runat="Server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
    Browser is <%# Request.Browser.Browser %> <br />
    MyProperty is "<%# MyPropertyInfo %>"<br />
    <asp:Label ID="Label2" runat="server" 
        Text="DataFromWebConfig(AppSetting-&gt;Author):"></asp:Label>
<asp:Label ID="lblAuthor" runat="server" Text="<%$ AppSettings:Author %>"></asp:Label>
    <br />
&nbsp;<asp:ListBox ID="ListBox1" runat="server"></asp:ListBox>
    <asp:ObjectDataSource ID="ObjectDataSource1" runat="server" 
        SelectMethod="GetProducts" TypeName="AspStudy">
        <SelectParameters>
            <asp:Parameter DefaultValue="1" Name="categoryId" Type="Int32" />
        </SelectParameters>
    </asp:ObjectDataSource>
    <asp:GridView ID="GridView1" runat="server" AllowSorting="True" 
        AutoGenerateEditButton="True" BackColor="#FF9933" BorderColor="#CCCCCC" 
        BorderStyle="Dotted" BorderWidth="2px" CellPadding="3" 
        DataSourceID="ObjectDataSource1" EnableSortingAndPagingCallbacks="True" 
        GridLines="None">
        <RowStyle ForeColor="#000066" />
        <FooterStyle BackColor="White" ForeColor="#000066" />
        <PagerStyle BackColor="White" ForeColor="#000066" HorizontalAlign="Left" />
        <SelectedRowStyle BackColor="#669999" Font-Bold="True" ForeColor="White" />
        <HeaderStyle BackColor="#006699" Font-Bold="True" ForeColor="White" />
    </asp:GridView>
</asp:Content>

