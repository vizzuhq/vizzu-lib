import { data } from "../../../../test_data/music_industry_history_1.mjs";

const description = `This is a 2-step animation:
1st: 
- set the Split parameter to true
- set the Align parameter to 'min'
2nd:
- move the Dimension from the Y-axis to the X-axis (before the other)
- set the Split parameter to false`;
const testSteps = [
  (chart) => {
    chart.on("plot-axis-label-draw", (event) => {
      let year = parseFloat(event.data.text);
      if (!event.data.text.includes("$") && !isNaN(year) && year % 5 != 0)
        event.preventDefault();
    });
    return chart;
  },

  (chart) =>
    chart.animate({
      data: Object.assign(data, {
        filter: (record) => record["Year"] > 1972,
      }),
      config: {
        channels: {
          x: "Year",
          y: ["Revenue [$]", "Format"],
          color: "Format",
        },
        title: "Title",
        geometry: "area",
        align: "center",
      },
      style: {
        plot: {
          yAxis: {
            label: {
              numberScale: "K, M, B, T",
            },
          },
        },
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        title: "Title",
        align: "min",
        split: true,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: ["Format", "Year"],
          y: "Revenue [$]",
        },
        title: "Title",
        split: false,
      },
    }),
  (chart) => chart.feature("tooltip", true),
];

export default testSteps;
