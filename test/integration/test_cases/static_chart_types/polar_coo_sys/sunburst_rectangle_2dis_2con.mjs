import { data } from "../../../test_data/sunburst.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          x: ["Joy factors/Country", "Value 2 (+)"],
          y: ["Type", "index"],
          label: "Joy factors/Country",
          color: "Joy factors/Country",
          lightness: "Value 5 (+/-)",
        },
        title: "Sunburst chart (a hierarchikus adatok szétszedése, még hack!)",
        coordSystem: "polar",
      },
      style: {
        plot: {
          marker: {
            label: {
              orientation: "tangential",
              angle: 3.14 * -0.5,
            },
          },
          yAxis: {
            title: {
              color: "#ffffff00",
            },
            label: {
              color: "#ffffff00",
            },
          },
        },
      },
    }),
];

export default testSteps;
