import { data } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          x: "Joy factors",
          y: "Value 2 (+)",
          label: "Value 2 (+)",
        },
        title: "Column Chart",
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
