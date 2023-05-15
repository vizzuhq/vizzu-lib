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
        coordSystem: "polar",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Year", "Value 2 (+)"] },
          y: { set: ["Country"] },
          color: { set: ["Country"] },
        },
        title: "Stacked Area Chart Vertical",
        geometry: "area",
        orientation: "vertical",
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
