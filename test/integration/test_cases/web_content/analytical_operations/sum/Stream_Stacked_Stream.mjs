import { data } from "../../../../test_data/music_industry_history_1.mjs";

const description = `This is a 2-step animation:

1st: set the Split parameter to true

2nd:
- move the Measure to the X-axis
- set the Align parameter 'center'
- set the Split parameter to false
- arrange the markers in reverse order
- switch the Geometry from Area to Rectangle`;
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
        
        split: true,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          y: ["Revenue [$]", "Year"],
          x: ["Format"],
        },
        
        geometry: "rectangle",
        align: "min",
        split: false,
        sort: "byValue",
        reverse: true,
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export { description };
export default testSteps;
