const Data1D = {
  data_1d_1: {
    input: {
      dimensions: [
        {
          name: "xt",
          values: ["AA", "BB", "CC"],
        },
      ],
      measures: [
        {
          name: "Foo",
          values: [1, 2, 3],
        },
        {
          name: "Bar",
          type: "measure",
          values: [41, 42, 43],
        },
      ],
      filter: () => {},
    },
    output: {
      series: [
        {
          name: "xt",
          type: "dimension",
          values: ["AA", "BB", "CC"],
        },
        {
          name: "Foo",
          values: [1, 2, 3],
        },
        {
          name: "Bar",
          type: "measure",
          values: [41, 42, 43],
        },
      ],
      filter: () => {},
    },
  },
  data_1d_2: {
    input: {
      dimensions: [
        {
          name: "xt",
          values: ["AA", "BB", "CC"],
        },
      ],
      measures: {
        name: "Foo",
        values: [1, 2, 3],
      },
    },
    output: {
      series: [
        {
          name: "xt",
          type: "dimension",
          values: ["AA", "BB", "CC"],
        },
        {
          name: "Foo",
          values: [1, 2, 3],
        },
      ],
    },
  },
  data_1d_3: {
    input: {
      dimensions: [
        {
          name: "xt",
          values: ["A", "B", "C"],
        },
      ],
      measures: {
        name: "Foo",
        values: [1, 2, 3],
      },
    },
    output: {
      series: [
        {
          name: "xt",
          type: "dimension",
          values: ["A", "B", "C"],
        },
        {
          name: "Foo",
          values: [1, 2, 3],
        },
      ],
    },
  },
  data_1d_4: {
    input: {
      dimensions: [
        {
          name: "xt",
          types: null,
          values: ["A", "B", "C"],
        },
      ],
      measures: {
        name: "Foo",
        values: [1, 2, 3],
      },
    },
    output: {
      series: [
        {
          name: "xt",
          type: "dimension",
          values: ["A", "B", "C"],
        },
        {
          name: "Foo",
          values: [1, 2, 3],
        },
      ],
    },
  },
  data_1d_5: {
    input: {
      dimensions: [
        {
          name: "xt",
          types: "dimension",
          values: ["A", "B", "C"],
        },
      ],
      measures: {
        name: "Foo",
        values: [1, 2, 3],
      },
    },
    output: {
      series: [
        {
          name: "xt",
          type: "dimension",
          values: ["A", "B", "C"],
        },
        {
          name: "Foo",
          values: [1, 2, 3],
        },
      ],
    },
  },
};

export default Data1D;
