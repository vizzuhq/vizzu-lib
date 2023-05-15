import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Year",
          y: "Value 2 (+)",
          color: "Country",
        },
        title: "Line Chart",
        geometry: "line",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: ["Value 2 (+)", "Country"],
        },
        title: "CHG Geom. & Stretch Markers to See Ratio of Categories",
        geometry: "area",
        align: "stretch",
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
