import { data } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          x: "Year",
          y: "Value 3 (+)",
          color: "Country",
        },
        title: "Lollipop Chart",
        align: "none",
        geometry: "circle",
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export default testSteps;
