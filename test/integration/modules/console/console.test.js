const Console = require("./console.js")
const path = require("path");


describe("new Console()", () => {
    test("if logFile is undefined", () => {
        let cnsl = new Console();
        const logFile = cnsl.getLogFile();
        expect(logFile).toBeUndefined();
    });

    test("if timeStamp is valid", () => {
        const startDate = new Date();
        let cnsl = new Console();
        const timeStamp = cnsl.getTimeStamp();
        expect(timeStamp.length).toBe(15);
        const separator = timeStamp.substring(8,9);
        expect(separator).toBe("_");
        const year = timeStamp.substring(0,4);
        const month = timeStamp.substring(4,6) - 1;
        const day = timeStamp.substring(6,8);
        const hour = timeStamp.substring(9,11);
        const min = timeStamp.substring(11,13);
        const sec = timeStamp.substring(13,15);
        const timeStampDateMin = new Date(year, month, day, hour, min, sec, 0);
        const timeStampDateMax = new Date(year, month, day, hour, min, sec, 999);
        const endDate = new Date();
        expect(timeStampDateMin).toBeBeforeOrEqualTo(endDate);
        expect(timeStampDateMax).toBeAfterOrEqualTo(startDate);
    });
});


describe("new Console(logPrefix)", () => {
    test("if logFile's dirname is __dirname", () => {
        let cnsl = new Console(logPrefix);
        const logFile = cnsl.getLogFile();
        expect(path.dirname(logFile)).toBe(path.join(__dirname, "../.."));
    });

    const logPrefix = "logPrefix";
    const cwdSaved = process.cwd()
    process.chdir("../");
    const cwd = process.cwd()
    let cnsl = new Console(logPrefix);
    const logFile = cnsl.getLogFile();
    process.chdir(cwdSaved);
    test("if logFile's dirname is cwd", () => {
        expect(path.dirname(logFile)).toBe(cwd);
    });

    test("if logFile's basename starts with logPrefix", () => {
        expect(path.basename(logFile)).toStartWith("logPrefix");
    });
});


describe("new Console(logPrefix, logPath)", () => {
    const logPrefix = "logPrefix";
    const logPath = "./logPath";
    let cnsl = new Console(logPrefix, logPath);
    const logFile = cnsl.getLogFile();
    const timeStamp = cnsl.getTimeStamp();

    test("if logFile is logPath/logPrefix_timeStamp.log", () => {
        expect(logFile).toBe(path.join(logPath, logPrefix + "_" + timeStamp + ".log"));
    });
});


describe("new Console().cnsl.log()", () => {

    test("if cnsl.log() does not log", () => {
        let cnsl = new Console();
        return cnsl.log().catch(e => expect(e).toMatch("parameter is required"));
    });

    test("if cnsl.log(\"Hello World\") logs \"Hello World\"", () => {
        const msg = "Hello World";
        let cnsl = new Console();
        const somethingSpy = jest.spyOn(console, "log").mockImplementation(() => {});
        cnsl.log(msg);
        expect(somethingSpy).toBeCalledWith(msg);
    });

});

