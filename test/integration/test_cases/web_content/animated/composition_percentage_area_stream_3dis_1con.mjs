import { data_14 } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_14,
      config: {
        channels: {
          x: "Year",
          y: ["Value 2 (+)", "Country"],
          color: "Country",
        },
        title: "Stacked Area Chart",
        geometry: "area",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        title: "100% Stacked Area Chart",
        align: "stretch",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: {
            /* Making the chart elements fill the whole of the y-axis
                    as the default value is now 110% */
            range: {
              max: "100%",
            },
          },
        },
        title: "Split Area Chart",
        align: "min",
        split: true,
      },
    }),
];

export default testSteps;
