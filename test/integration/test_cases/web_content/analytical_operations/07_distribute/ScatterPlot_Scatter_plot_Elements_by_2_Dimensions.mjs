import { data } from "../../../../test_data/chart_types_eu.mjs";

const description = `This Vzzu animation has 4 simple steps (4 animates):
1st: replace the Measure on the X-axis to the Dimension whitch is on the Lightness channel
2nd: back
3rd: replace the Measure on the Y-axis to the Dimension whitch is on the Noop channel
4th: back`;
const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          x: "Value 5 (+/-)",
          y: "Value 6 (+/-)",
          /* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
          noop: "Joy factors",
          /* Lightness channel is used to assist the viewer
                in following the animation. */
          lightness: "Year",
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
        },
        title: "Title",
        orientation: "horizontal",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: "Value 5 (+/-)",
        },
        title: "Title",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: "Joy factors",
          noop: null,
        },
        title: "Title",
        orientation: "vertical",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: "Value 6 (+/-)",
          noop: "Joy factors",
        },
        title: "Title",
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export default testSteps;
