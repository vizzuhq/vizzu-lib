import { data_6 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- move one Dimension from X-axis to the Y-axis (stack)`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_6,

      config: {
        channels: {
          x: ["Year", "Country"],
          y: "Value 2 (+)",
          color: "Country",
        },
        title: "Title",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: "Year",
          y: ["Country", "Value 2 (+)"],
          color: "Country",
        },
        title: "Title",
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export { description };
export default testSteps;
