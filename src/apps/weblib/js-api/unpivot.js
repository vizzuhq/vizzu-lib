const assert = (condition, message) => {
  if (!condition) {
    throw new Error("Assert failed: " + message);
  }
};

const assertArray = (data, array, index) => {
  assert(Array.isArray(array), "array is not a list");
  try {
    if (array.length !== data.dimensions[index].values.length) {
      throw new Error("Assert failed");
    }
  } catch (err) {
    assert(false, "dimensions are not the same");
  }
  array.forEach((item) => {
    if (Array.isArray(item)) {
      assertArray(data, item, index - 1);
    }
  });
};

export default class UnPivot {
  static isPivot(data) {
    return (
      typeof data?.dimensions !== "undefined" ||
      typeof data?.measures !== "undefined"
    );
  }

  static convert(data) {
    assert(
      typeof data === "object" && data !== null && !Array.isArray(data),
      "data is not an object"
    );
    assert("dimensions" in data, "data.dimensions is requreid");
    assert("measures" in data, "data.measures is requreid");

    data.series = [];

    let dimensionsProduct = 1;
    assert(Array.isArray(data.dimensions), "data.dimensions is not a list");
    for (let i = 0; i < data.dimensions.length; i++) {
      let item = data.dimensions[i];
      assert(
        typeof item === "object" && item !== null && !Array.isArray(item),
        "data.dimensions.item is not an object"
      );
      assert("name" in item, "data.dimensions.item.name is requreid");
      assert(
        typeof item.type === "undefined" || item.type === "dimension",
        "data.dimensions.item.type is invalid"
      );
      assert("values" in item, "data.dimensions.item.values is requreid");
      assert(
        Array.isArray(item.values),
        "data.dimensions.item.values is not a list"
      );
      assert(
        item.values.length !== 0,
        "data.dimensions.item.values length is zero"
      );
      dimensionsProduct *= item.values.length;
    }

    // TODO check
    let dimensionsProductProcessed = 1;
    for (let i = 0; i < data.dimensions.length; i++) {
      let item = data.dimensions[i];
      let values = [];
      let valuesItem = [];
      item.values.forEach((value) => {
        for (let b = 1; b <= dimensionsProductProcessed; b++) {
          valuesItem.push(value);
        }
      });
      dimensionsProductProcessed *= item.values.length;
      for (
        let a = 1;
        a <= dimensionsProduct / dimensionsProductProcessed;
        a++
      ) {
        values = values.concat(valuesItem);
      }

      let seriesItem = {
        name: item.name,
        type: item.type || "dimension",
        values: values,
      };
      data.series.push(seriesItem);
    }

    assert(
      typeof data.measures === "object" && data.measures !== null,
      "data.measures is not a list or an object"
    );
    if (!Array.isArray(data.measures)) {
      data.measures = [data.measures];
    }
    for (let i = 0; i < data.measures.length; i++) {
      let item = data.measures[i];
      assert(
        typeof item === "object" && item !== null && !Array.isArray(item),
        "data.measures.item is not an object"
      );
      assert("name" in item, "data.measures.item.name is requreid");
      assert("values" in item, "data.measures.item.values is requreid");
      assert(
        Array.isArray(item.values),
        "data.measures.item.values is not a list"
      );
      assertArray(data, item.values, data.dimensions.length - 1);
      let seriesItem = {
        name: item.name,
        type: item.type,
        values: item.values.flat(Infinity),
      };
      if (typeof seriesItem.type === "undefined") {
        delete seriesItem.type;
      }
      assert(
        seriesItem.values.length === dimensionsProduct,
        "dimensions are not the same"
      );
      data.series.push(seriesItem);
    }
  }
}
