import { data_14 } from "../../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_14,
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 2 (+)" },
          color: { set: "Country" },
        },
        title: "Line Chart",
        geometry: "line",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: null },
          y: { set: null },
          color: { set: "Country" },
          size: { set: ["Joy factors", "Value 3 (+)"] },
        },
        title: "Stack new Disc & Change Geoms & CoordSys",
        geometry: "circle",
      },
    }),
];

export default testSteps;
