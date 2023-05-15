import { data_14 } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_14,
      config: {
        channels: {
          x: { set: ["Year"] },
          y: { set: ["Value 2 (+)"] },
          color: { set: ["Country"] },
        },
        title: "Line Chart",
        geometry: "line",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Value 2 (+)"] },
          noop: { set: ["Year"] },
          y: { set: ["Country"] },
          color: { set: ["Country"] },
        },
        title: "Dotplot",
        geometry: "circle",
        orientation: "horizontal",
        split: false,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Year"] },
          y: { set: ["Value 2 (+)"] },
          color: { set: ["Country"] },
        },
        title: "Line Chart",
        geometry: "line",
        legend: null,
      },
    }),
];

export default testSteps;
