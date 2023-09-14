import { data } from "../../../../test_data/music_industry_history_1.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: chart.constructor.presets.violin({
        x: "Year",
        y: "Revenue",
        splittedBy: "Format",
        title: "Violin Graph",
      }),
      style: {
        plot: {
          yAxis: { interlacing: { color: "#ffffff00" } },
          yAxis: { label: { numberScale: "K, M, B, T" } },
          xAxis: { label: { angle: "-45deg" } },
        },
      },
    }),
];

export default testSteps;
