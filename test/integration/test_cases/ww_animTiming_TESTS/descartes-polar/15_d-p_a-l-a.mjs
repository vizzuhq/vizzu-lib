import { data_14 } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_14,
      config: {
        channels: {
          x: { set: ["Year"] },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: ["Country"] },
        },
        title: "Stacked Area Chart",
        geometry: "area",
        legend: null,
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
        title: "Radial Line Chart Vertical",
        geometry: "line",
        coordSystem: "polar",
        orientation: "vertical",
        split: false,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Year"] },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: ["Country"] },
        },
        title: "Stacked Area Chart",
        geometry: "area",
        coordSystem: "cartesian",
        legend: null,
      },
    }),
];

export default testSteps;
