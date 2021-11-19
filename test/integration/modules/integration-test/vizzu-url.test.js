const VizzuUrl = require("./vizzu-url.js")
const path = require("path");
const fs = require("fs");
const fetch = require("node-fetch");


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
                expect(url).toBe(VizzuUrl.getRemoteStableBucket() + "/lib" + VizzuUrl.getVizzuMinJs());
            });
        });

        test("if head/", () => {
            return VizzuUrl.resolveVizzuUrl("head/").then(url => {
                expect(url).toBe(VizzuUrl.getRemoteStableBucket() + "/lib" + VizzuUrl.getVizzuMinJs());
            });
        });

        test("if head" + VizzuUrl.getVizzuMinJs(), () => {
            return VizzuUrl.resolveVizzuUrl("head" + VizzuUrl.getVizzuMinJs()).then(url => {
                expect(url).toBe(VizzuUrl.getRemoteStableBucket() + "/lib" + VizzuUrl.getVizzuMinJs());
            });
        });

        test("if head" + VizzuUrl.getVizzuJs(), () => {
            return VizzuUrl.resolveVizzuUrl("head" + VizzuUrl.getVizzuJs()).then(url => {
                expect(url).toBe(VizzuUrl.getRemoteStableBucket() + "/lib" + VizzuUrl.getVizzuJs());
            });
        });
    });

    describe("sha", () => {
        test("if sha is not exist", () => {
            return VizzuUrl.resolveVizzuUrl("0000000").catch(e => expect(e.toString()).toMatch("404"));
        });

        let shaReady = fetch(VizzuUrl.getRemoteStableBucket() + '/lib/sha.txt').then(shaRaw => {
            return shaRaw.text()
        });

        test("if sha", () => {
            return shaReady.then(sha => {
                sha = sha.toString().trim();
                VizzuUrl.resolveVizzuUrl(sha).then(url => {
                    expect(url).toBe(VizzuUrl.getRemoteBucket() + "/lib-" + sha + VizzuUrl.getVizzuMinJs());
                });
            });
        });

        test("if sha/", () => {
            return shaReady.then(sha => {
                sha = sha.toString().trim();
                VizzuUrl.resolveVizzuUrl(sha + "/").then(url => {
                    expect(url).toBe(VizzuUrl.getRemoteBucket() + "/lib-" + sha + VizzuUrl.getVizzuMinJs());
                });
            });
        });

        test("if sha" + VizzuUrl.getVizzuMinJs(), () => {
            return shaReady.then(sha => {
                sha = sha.toString().trim();
                VizzuUrl.resolveVizzuUrl(sha + VizzuUrl.getVizzuMinJs()).then(url => {
                    expect(url).toBe(VizzuUrl.getRemoteBucket() + "/lib-" + sha + VizzuUrl.getVizzuMinJs());
                });
            });
        });

        test("if sha" + VizzuUrl.getVizzuJs(), () => {
            return shaReady.then(sha => {
                sha = sha.toString().trim();
                VizzuUrl.resolveVizzuUrl(sha + VizzuUrl.getVizzuJs()).then(url => {
                    expect(url).toBe(VizzuUrl.getRemoteBucket() + "/lib-" + sha + VizzuUrl.getVizzuJs());
                });
            });
        });
    });

    describe("cdn", () => {

        let cdn = "0.0.0";

        test("if " + cdn + " err is thrown", () => {
            return VizzuUrl.resolveVizzuUrl(cdn).catch(e => expect(e.toString()).toMatch("404"));
        });

        cdn = "0.3.0";

        test("if " + cdn, () => {
            return VizzuUrl.resolveVizzuUrl(cdn).then(url => {
                expect(url).toBe(VizzuUrl.getRemoteCdn() + "@" + cdn + "/dist" + VizzuUrl.getVizzuMinJs());
            });
        });

        test("if " + cdn + "/", () => {
            return VizzuUrl.resolveVizzuUrl(cdn + "/").then(url => {
                expect(url).toBe(VizzuUrl.getRemoteCdn() + "@" + cdn + "/dist" + VizzuUrl.getVizzuMinJs());
            });
        });

        test("if " + cdn + VizzuUrl.getVizzuMinJs(), () => {
            return VizzuUrl.resolveVizzuUrl(cdn + VizzuUrl.getVizzuMinJs()).then(url => {
                expect(url).toBe(VizzuUrl.getRemoteCdn() + "@" + cdn + "/dist" + VizzuUrl.getVizzuMinJs());
            });
        });

        test("if " + cdn + VizzuUrl.getVizzuJs(), () => {
            return VizzuUrl.resolveVizzuUrl(cdn + VizzuUrl.getVizzuJs()).catch(e => 
                expect(e.toString()).toMatch("select Vizzu from cdn can be used with vizzu.min.js only"));
        });
    });

    describe("local", () => {
        test("if err is thrown (root and dirname are undefined)", () => {
            return VizzuUrl.resolveVizzuUrl("/").catch(e => expect(e.toString()).toMatch("Error: parameter is required"));
        });

        test("if err is thrown (root or dirname is undefined)", () => {
            return VizzuUrl.resolveVizzuUrl("/").catch(e => expect(e.toString()).toMatch("Error: parameter is required"));
        });

        test("if err is thrown (local does not exist)", () => {
            return VizzuUrl.resolveVizzuUrl("/", "/", ".").catch(e => expect(e.toString()).toMatch("Error: ENOENT: no such file or directory, stat '/vizzu.js'"));
        });

        describe("local is exists", () => {
            beforeAll(() => {
                return new Promise((resolve, reject) => {
                    let local = "./test_report";
                    fs.mkdir(local, { recursive: true, force: true }, err => {
                        if (err) {
                            return reject(err);
                        }
                        let vizzuJsReady = new Promise((resolve, reject) => {
                            fs.open(local + VizzuUrl.getVizzuJs(), "w", (err, file) => {
                                if (err) {
                                    return reject(err);
                                }
                                return resolve();
                            });
                        });
                        let vizzuMinJsReady = new Promise((resolve, reject) => {
                            fs.open(local + VizzuUrl.getVizzuMinJs(), "w", (err, file) => {
                                if (err) {
                                    return reject(err);
                                }
                                return resolve();
                            });
                        });
                        Promise.all([vizzuJsReady, vizzuMinJsReady]).then(() => {
                            return resolve();
                        });
                    });
                });
            });

            describe("with relative path", () => {
                let local1 = "./test_report";
                test("if " + local1, () => {
                    return VizzuUrl.resolveVizzuUrl(local1, "../..", ".").then(url => {
                        expect(url).toBe(path.resolve(local1 + VizzuUrl.getVizzuJs()));
                    });
                });

                let local2 = "./test_report/";
                test("if " + local2, () => {
                    return VizzuUrl.resolveVizzuUrl(local2, "../..", ".").then(url => {
                        expect(url).toBe(path.resolve(local2 + "vizzu.js"));
                    });
                });

                let local3 = "./test_report" + VizzuUrl.getVizzuJs();
                test("if " + local3, () => {
                    return VizzuUrl.resolveVizzuUrl(local3, "../..", ".").then(url => {
                        expect(url).toBe(path.resolve(local3));
                    });
                });

                let local4 = "./test_report" + VizzuUrl.getVizzuMinJs();
                test("if " + local4, () => {
                    return VizzuUrl.resolveVizzuUrl(local4, "../..", ".").then(url => {
                        expect(url).toBe(path.resolve(local4));
                    });
                });
            });

            describe("with absolute path", () => {
                let local1 = "/test/integration/test_report";
                test("if " + local1, () => {
                    return VizzuUrl.resolveVizzuUrl(local1, "../..", ".").then(url => {
                        expect(url).toBe(path.resolve("../.." + local1 + VizzuUrl.getVizzuJs()));
                    });
                });

                let local2 = "/test/integration/test_report/";
                test("if " + local2, () => {
                    return VizzuUrl.resolveVizzuUrl(local2, "../..", ".").then(url => {
                        expect(url).toBe(path.resolve("../.." + local2 + "vizzu.js"));
                    });
                });

                let local3 = "/test/integration/test_report" + VizzuUrl.getVizzuJs();
                test("if " + local3, () => {
                    return VizzuUrl.resolveVizzuUrl(local3, "../..", ".").then(url => {
                        expect(url).toBe(path.resolve("../.." + local3));
                    });
                });

                let local4 = "/test/integration/test_report" + VizzuUrl.getVizzuMinJs();
                test("if " + local4, () => {
                    return VizzuUrl.resolveVizzuUrl(local4, "../..", ".").then(url => {
                        expect(url).toBe(path.resolve("../.." + local4));
                    });
                });
            });

            afterAll(() => {
                return new Promise((resolve, reject) => {
                    let local = "./test_report";
                    let rmVizzuJsReady = new Promise((resolve, reject) => {
                        fs.rm(local + VizzuUrl.getVizzuJs(), { force: true }, err => {
                            if (err) {
                                return reject(err);
                            }
                            return resolve();
                        });
                    });
                    let rmVizzuMinJsReady = new Promise((resolve, reject) => {
                        fs.rm(local + VizzuUrl.getVizzuMinJs(), { force: true }, err => {
                            if (err) {
                                return reject(err);
                            }
                            return resolve();
                        });
                    });
                    Promise.all([rmVizzuJsReady, rmVizzuMinJsReady]).then(() => {
                        return resolve();
                    });
                });
            });
        });
    });
});
