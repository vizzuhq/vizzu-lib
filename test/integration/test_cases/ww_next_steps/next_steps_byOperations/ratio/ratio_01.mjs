import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Year",
          y: ["Country", "Value 2 (+)"],
          color: "Country",
        },
        title: "Stacked Column",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        title: "Stretch Markers to See Ratio of Categories",
        align: "stretch",
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
