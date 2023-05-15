var DataCollection = {
  simpleDataWithOneValue: {
    series: [
      {
        name: "Foo",
        type: "dimension",
        values: ["Baz"],
      },
      {
        name: "Bar",
        type: "measure",
        values: [42],
      },
    ],
  },
  simpleDataWithThreeValue: {
    series: [
      {
        name: "Foo",
        values: [42, 43, 44],
      },
      {
        name: "Bar",
        values: [2, 3, 4],
      },
    ],
  },
};

export default DataCollection;
