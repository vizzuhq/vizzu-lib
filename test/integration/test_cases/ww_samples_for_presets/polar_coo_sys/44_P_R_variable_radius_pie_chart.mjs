import { data } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          x: ["Joy factors", "Value 2 (+)"],
          y: "Value 1 (+)",
          color: "Joy factors",
          label: "Value 1 (+)",
        },
        title: "Variable Radius Pie Chart",
        coordSystem: "polar",
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
