const VizzuUrl = require("./vizzu-url.js")
const path = require("path");
const fs = require("fs");


test("if getRemoteBucket()", () => {
    expect(VizzuUrl.getRemoteBucket()).toBe("https://vizzu-lib-main-sha.storage.googleapis.com");
});

test("if getRemoteStableBucket()", () => {
    expect(VizzuUrl.getRemoteStableBucket()).toBe("https://vizzu-lib-main.storage.googleapis.com");
});

test("if getRemoteCdn()", () => {
    expect(VizzuUrl.getRemoteCdn()).toBe("https://cdn.jsdelivr.net/npm/vizzu");
});

test("if getVizzuJs()", () => {
    expect(VizzuUrl.getVizzuJs()).toBe("/vizzu.js");
});

test("if getVizzuMinJs()", () => {
    expect(VizzuUrl.getVizzuMinJs()).toBe("/vizzu.min.js");
});

describe("resolveVizzuUrl()", () => {
    describe("head", () => {
        test("if head", () => {
            return VizzuUrl.resolveVizzuUrl("head").then(url => {
                expect(url).toBe("https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.min.js");
            });
        });

        test("if head/", () => {
            return VizzuUrl.resolveVizzuUrl("head/").then(url => {
                expect(url).toBe("https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.min.js");
            });
        });

        test("if head/vizzu.min.js", () => {
            return VizzuUrl.resolveVizzuUrl("head/vizzu.min.js").then(url => {
                expect(url).toBe("https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.min.js");
            });
        });

        test("if head/vizzu.js", () => {
            return VizzuUrl.resolveVizzuUrl("head/vizzu.js").then(url => {
                expect(url).toBe("https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js");
            });
        });
    });

    describe("sha", () => {
        test("if sha is not exist", () => {
            return VizzuUrl.resolveVizzuUrl("0000000").catch(e => expect(e.toString()).toMatch("404"));
        });
    });
});
