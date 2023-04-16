import { data_6 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- remove the Dimension from the Label channel and add the othere Dimension to it`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_6,

      config: {
        channels: {
          x: "Value 1 (+)",
          y: "Value 3 (+)",
          size: "Value 2 (+)",
          color: "Year",
          label: "Country_code",
        },
        title: "Title",
        geometry: "circle",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          label: "Year",
        },
        title: "Title",
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export default testSteps;
