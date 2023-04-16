import { data_6 } from "../../../../test_data/chart_types_eu.mjs";

const description = `- replace the Dimension on the Color channel with a new one
- + here is an example how you can add a color palette to the Style`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data_6,

      config: {
        channels: {
          x: "Year",
          y: "Value 2 (+)",
          color: "Joy factors",
        },
        title: "Title",
        geometry: "line",
        coordSystem: "polar",
      },
      style: {
        plot: {
          marker: {
            colorPalette: "#ef675aFF #6d8cccFF #e6cf99FF #9c50abFF",
          },
        },
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          color: "Country",
        },
        title: "Title",
      },
      style: {
        plot: {
          marker: {
            colorPalette: null,
          },
        },
      },
    }),
];

export default testSteps;
