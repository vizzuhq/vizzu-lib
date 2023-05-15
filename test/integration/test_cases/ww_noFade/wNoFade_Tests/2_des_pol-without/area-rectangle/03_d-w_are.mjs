import { data_14 } from "../../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_14,
      config: {
        channels: {
          x: "Year",
          y: ["Country", "Value 2 (+)"],
          color: "Country",
        },
        title: "Stacked Area Chart",
        geometry: "area",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: null,
          y: null,
          color: "Country",
          noop: "Year",
          size: ["Year", "Value 3 (+)"],
        },
        title: "Change Geoms & coordSys.",
        geometry: "rectangle",
      },
    }),
];

export default testSteps;
