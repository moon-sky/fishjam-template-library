"use strict";
/*************************************************************************************************************************
* var fr = new FileReader()
* fr.readAsArrayBuffer(fileSlice(file, e.data.position, e.data.amount));
*************************************************************************************************************************/


module("FileTester", {
    setup: function() {
    //dump("in FileTester::setup\n");
    },
    teardown: function() {
    //dump("in FileTester::teardown\n");
    }
});

test("File", function() {
    var fr = new FileReader();
    console.log("%o", fr);
    equal(1, 1, "TODO:JSÖ§³ÖFileÂð");
});
