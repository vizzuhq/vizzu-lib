import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Value 5 (+/-)",
          y: "Value 2 (+)",
          color: "Country",
          noop: "Year",
        },
        title: "Scatter plot",
        geometry: "circle",
        split: false,
      },
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: "Year",
          y: ["Country", "Value 2 (+)"],
          noop: null,
          label: "Value 2 (+)",
        },
        title: "If Remove a Measure, then a Splitted Column is Better",
        geometry: "rectangle",
        split: true,
      },
      style: {
        plot: {
          marker: {
            label: {
              position: "top",
              fontSize: "0.6em",
            },
          },
        },
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
