/*************************************************************************************************************************
JavaScript存储
*************************************************************************************************************************/


module("StorageTester", {
    setup: function() {
    //dump("in StorageTester::setup\n");
    },
    teardown: function() {
    //dump("in StorageTester::teardown\n");
    }
});

test("localStorage", function() {
    equal(1, 1, "TODO:localStorage");

    var TaobaoInfo =
    [
        { "username": "fishjam", "cart": "电话卡,手机" },
        { "username": "hujin", "cart": "衣服" }
    ];

    window.localStorage["taobaoInfo"] = TaobaoInfo;
    console.log(window.localStorage);


    var readTaobaoInfo = window.localStorage["taobaoInfo"];
    console.log(readTaobaoInfo);
    //equal(readTaobaoInfo.username, "fishjam");

    window.localStorage["myBirthday"] = 1979;
    equal(window.localStorage["myBirthday"], 1979, "生日");
});
