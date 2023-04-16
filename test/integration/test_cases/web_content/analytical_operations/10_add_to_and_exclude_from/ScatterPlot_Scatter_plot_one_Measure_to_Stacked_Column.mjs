import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- replace X-axis Measure with no-operational (Noop) channel Dimension
- move Color channel Dimension to Y-axis too
- switch the Geometry from Circle to Rectangle`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Value 5 (+/-)",
          y: "Value 2 (+)",
          color: "Country",
          noop: "Year",
        },
        title: "Title",
        geometry: "circle",
      },
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: "Year",
          y: ["Country", "Value 2 (+)"],
          noop: null,
        },
        title: "Title",
        geometry: "rectangle",
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export { description };
export default testSteps;
