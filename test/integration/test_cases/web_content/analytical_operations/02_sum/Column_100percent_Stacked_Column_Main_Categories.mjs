import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- set Align parameter from 'stretch' to 'min'`;
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
        title: "Title",
        align: "stretch",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        title: "Title",
        align: "min",
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export default testSteps;
