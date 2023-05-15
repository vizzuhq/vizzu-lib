const testData = {
  series: [
    {
      name: "Colors",
      type: "dimension",
      values: ["red", "green", "blue"],
    },
    {
      name: "Val",
      type: "measure",
      values: [3, 5, 4],
    },
  ],
};

const testSteps = [
  (chart) =>
    chart.animate({
      data: testData,
      config: {
        channels: {
          x: { attach: ["Colors"] },
        },
        title: null,
        legend: null,
      },
    }),
  (chart) =>
    chart.animate({
      data: {
        filter: (record) => record.Colors != "blue",
      },
      config: {
        channels: {
          x: { detach: ["Colors"] },
          y: { attach: ["Colors"] },
        },
      },
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          color: { attach: ["Colors"] },
        },
      },
    }),
  (chart) =>
    chart.animate({
      data: {
        filter: null,
      },
      config: {
        channels: {
          color: { detach: ["Colors"] },
          lightness: { attach: ["Colors"] },
        },
      },
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          lightness: { detach: ["Colors"] },
          label: { attach: ["Colors"] },
        },
      },
    }),
];

export default testSteps;
