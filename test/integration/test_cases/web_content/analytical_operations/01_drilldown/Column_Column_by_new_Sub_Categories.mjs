import { data_8 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- add 2 new Dimensions to the Y-axis and the Color channel`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_8,

      config: {
        channels: {
          x: "Country",
          y: "Value 2 (+)",
          label: "Value 2 (+)",
        },
        title: "Title",
      },
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: ["Value 2 (+)", "Joy factors"],
          color: "Joy factors",
        },
        title: "Title",
      },
      style: {
        plot: {
          marker: {
            colorPalette: "#ef675aFF #6d8cccFF #e6cf99FF #9c50abFF",
          },
        },
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export { description };
export default testSteps;
