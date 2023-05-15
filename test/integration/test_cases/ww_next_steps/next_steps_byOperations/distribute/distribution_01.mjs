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
        channels: {
          y: "Value 2 (+)",
        },
        title: "Change Geometry & Unstack to See Distribution of Values",
        geometry: "circle",
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
