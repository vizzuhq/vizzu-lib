import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- set the Split parameter to true`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Year",
          y: { set: ["Country", "Value 2 (+)"], range: { min: "-20%" } },
          color: "Country",
        },
        title: "Title",
        geometry: "area",
        coordSystem: "polar",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        title: "Title",
        split: true,
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export { description };
export default testSteps;
