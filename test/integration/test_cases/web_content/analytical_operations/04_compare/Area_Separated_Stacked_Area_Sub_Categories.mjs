import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- remove the Dimension from Y-axis a
- set the Split parameter from true to false
- switch the Geometry from Area to Line`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Year",
          y: ["Value 2 (+)", "Country"],
          color: "Country",
        },
        title: "Title",
        geometry: "area",
        split: true,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: "Value 2 (+)",
        },
        title: "Title",
        geometry: "line",
        split: false,
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export { description };
export default testSteps;
