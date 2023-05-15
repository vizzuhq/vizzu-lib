import { data } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          y: "Value 2 (+)",
          x: "Joy factors",
          color: "Joy factors",
          label: "Value 2 (+)",
        },
        title: "Coxcomb Chart",
        orientation: "horizontal",
        coordSystem: "polar",
      },
      style: {
        plot: {
          marker: {
            rectangleSpacing: 0,
          },
        },
      },
    }),
];

export default testSteps;
