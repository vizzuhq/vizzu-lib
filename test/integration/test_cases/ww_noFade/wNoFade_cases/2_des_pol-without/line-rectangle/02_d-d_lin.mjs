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
      style: {
        plot: {
          marker: { lineMaxWidth: 0.02 },
        },
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 3 (+)" },
          color: { set: "Country" },
        },
        title: "Change Conti",
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: ["Country", "Value 3 (+)"] },
            color: { set: "Country" },
          },
          title: "Change Geoms & CoordSys",
          geometry: "rectangle",
        },
      },
      {
        geometry: {
          delay: 0.5,
          duration: 1.5,
        },
        x: {
          delay: 1,
        },
      }
    ),
];

export default testSteps;
