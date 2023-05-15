import { data_6 } from "../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_6,

      config: {
        channels: {
          x: "Year",
          y: ["Country", "Value 2 (+)"],
          color: "Country",
          label: "Value 2 (+)",
        },
        title: "Splitted Column",
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

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: ["Year", "Country"],
          y: "Value 2 (+)",
          label: null,
        },
        title: "Split off & Group Markers to Compare Values",
        split: false,
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
