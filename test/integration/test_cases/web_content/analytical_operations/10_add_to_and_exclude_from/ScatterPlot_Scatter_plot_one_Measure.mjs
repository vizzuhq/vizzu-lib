import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- remove the Measure from the Size and the Label channels`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Value 1 (+)",
          y: "Value 3 (+)",
          size: "Value 2 (+)",
          noop: "Year",
          label: ["Year", "Value 2 (+)"],
        },
        title: "Title",
        geometry: "circle",
        legend: "size",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          size: null,
          label: "Year",
        },
        title: "Title",
        //           legend: null
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export { description };
export default testSteps;
