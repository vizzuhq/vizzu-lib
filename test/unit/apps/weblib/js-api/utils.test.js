import {
  isAccessibleStylesheet,
  getCSSCustomProps,
  getCSSCustomPropsForElement,
  propSet,
  propGet,
  propsToObject,
} from "../../../../../src/apps/weblib/js-api/utils.js";

describe("utils.isAccessibleStylesheet()", () => {
  test("true -> can access `.cssRules`", () => {
    let stylesheet = { cssRules: [] };
    expect(isAccessibleStylesheet(stylesheet)).toBeTruthy();
  });

  test("false -> cannot access `.cssRules`", () => {
    let stylesheet = {
      get cssRules() {
        throw new Error("not accessible");
      },
    };
    expect(isAccessibleStylesheet(stylesheet)).toBeFalsy();
  });
});

describe("utils.getCSSCustomProps()", () => {
  test("no stylesheet, no props", () => {
    global.document = { styleSheets: [] }; // mockig document
    expect(getCSSCustomProps()).toEqual([]);
  });
  test("empty stylesheets, no props", () => {
    global.document = { styleSheets: [{ cssRules: [] }, { cssRules: [] }] };
    expect(getCSSCustomProps()).toEqual([]);
  });
  test("stylesheet with proper rules, props expected", () => {
    global.document = {
      styleSheets: [
        {
          cssRules: [
            { type: 1, style: ["--test-property", "--test-property-2"] },
          ],
        },
      ],
    };
    expect(getCSSCustomProps()).toEqual([
      "--test-property",
      "--test-property-2",
    ]);
  });
  test("stylesheet with proper rules, using prefix, props expected", () => {
    global.document = {
      styleSheets: [
        {
          cssRules: [
            { type: 1, style: ["--test-property", "--no-test-property"] },
          ],
        },
      ],
    };
    expect(getCSSCustomProps("test")).toEqual(["--test-property"]);
  });
});

describe("utils.getCSSCustomPropsForElement()", () => {
  test("only element related props should show up", () => {
    global.document = {
      styleSheets: [
        {
          cssRules: [
            { type: 1, style: ["--test-property", "--test-property-2"] },
          ],
        },
      ],
    };
    global.getComputedStyle = () => {
      return {
        getPropertyValue: (prop) => {
          if (prop === "--test-property") {
            return "test";
          }
          return "";
        },
      };
    };
    expect(getCSSCustomPropsForElement("whatever")).toEqual([
      ["--test-property", "test"],
    ]);
  });
});

describe("utils.propSet()", () => {
  test("set embedded property on empty object", () => {
    const obj = {};
    propSet(obj, ["alma", "beka", "cica"], "test");
    expect(obj?.alma?.beka?.cica).toEqual("test");
  });

  test("does not overwrite by default", () => {
    const obj = { alma: { beka: { cica: "notest" } } };
    propSet(obj, ["alma", "beka", "cica"], "test");
    expect(obj?.alma?.beka?.cica).toEqual("notest");
  });

  test("can overwrite if requested", () => {
    const obj = { alma: { beka: { cica: "notest" } } };
    propSet(obj, ["alma", "beka", "cica"], "test", true);
    expect(obj?.alma?.beka?.cica).toEqual("test");
  });
});

describe("utils.propGet()", () => {
  test("get embedded property", () => {
    const obj = { alma: { beka: { cica: "test" } } };
    expect(propGet(obj, ["alma", "beka", "cica"])).toEqual("test");
  });
});

describe("utils.propsToObject()", () => {
  test('generate "deep" object from property list', () => {
    const props = [["--alma-beka-cica", "test"]];
    const obj = propsToObject(props, null);
    expect(propGet(obj, ["alma", "beka", "cica"])).toEqual("test");
  });

  test('generate "deep" object from property list, using prefix', () => {
    const props = [["--test-alma-beka-cica", "test"]];
    const obj = propsToObject(props, null, "test");
    expect(propGet(obj, ["alma", "beka", "cica"])).toEqual("test");
  });

  test('fill existing "deep" object from property list, using prefix, no overwrite', () => {
    const props = [["--test-alma-beka-cica", "test"]];
    const obj = { alma: { beka: { cica: "notest" } } };
    propsToObject(props, obj, "test");
    expect(propGet(obj, ["alma", "beka", "cica"])).toEqual("notest");
  });

  test('fill existing "deep" object from property list, using prefix, overwrite', () => {
    const props = [["--test-alma-beka-cica", "test"]];
    const obj = { alma: { beka: { cica: "notest" } } };
    propsToObject(props, obj, "test", true);
    expect(propGet(obj, ["alma", "beka", "cica"])).toEqual("test");
  });
});
