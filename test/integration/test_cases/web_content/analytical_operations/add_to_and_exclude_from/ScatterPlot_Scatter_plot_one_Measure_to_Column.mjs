import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- replace the Measure on the X-axis with the Dimension, and remove it from the Color channel
- switch the Geometry from Circle to Rectangle`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Value 4 (+/-)",
          y: "Value 2 (+)",
          color: "Country",
          label: "Value 4 (+/-)",
        },
        title: "Title",
        geometry: "circle",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: "Country",
          color: null,
          label: "Value 2 (+)",
        },
        title: "Title",
        geometry: "rectangle",
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export { description };
export default testSteps;
