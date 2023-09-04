import { data } from "../../../test_data/music_industry_history_1.mjs";

const testSteps = [
  (chart) => {
    chart.on("plot-axis-label-draw", (event) => {
      let year = parseFloat(event.detail.text);
      if (!isNaN(year) && year % 5 != 0) event.preventDefault();
    });
    return chart.animate({
      data: data,
      config: {
        channels: {
          x: "Year",
          y: "Format",
          lightness: "Revenue [$]",
        },
        title: "Heatmap2",
      },
    });
  },
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
