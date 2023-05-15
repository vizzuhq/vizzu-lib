import { data_1974_1990 } from "../../../test_data/IMDB_data.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_1974_1990,
      config: {
        channels: {
          x: "Year",
          y: { set: "Index", range: { max: "100%" } },
        },
        title: "Matrix Chart",
        align: "none",
      },
      style: {
        plot: {
          marker: { borderOpacity: 0, rectangleSpacing: 0 },
        },
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
