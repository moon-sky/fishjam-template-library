$(function() {
    console.log("in popup.js");
    //$("submit").val(chrome.i18n.getMessage("__MSG_extSubmit_"))

    $("#btnSearch").click(function() {
        console.log(chrome.tabs);
        $("#result").load(
                "http://www.baidu.com/s",
                $("#form1").serialize(),
                function() {
                    $("#kw").val("after search from extension");
                    console.log("after search");
                });
        return false;
    });

    $("#btnClose").click(function() {
        close();
    });
});