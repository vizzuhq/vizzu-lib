import { data } from "../../../test_data/budapest_school.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: Object.assign(data, {
        filter: (record) => record.student_name == "Nyiri András Péter",
      }),
      config: {
        channels: {
          x: ["area"],
          y: ["outcome", "level"],
          color: "level",
        },
        title: "Nyiri András Péter",
      },
      style: {
        plot: {
          paddingLeft: "0.5em",
          yAxis: {
            label: {
              color: "#ffffffFF",
            },
          },
          xAxis: {
            label: {
              angle: -1.55,
            },
          },
          marker: {
            colorPalette: "#362d8bFF #6359c2FF #a89ef0FF #e2defcFF #00000000",
            borderOpacity: 0.5,
          },
        },
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: ["area"],
          y: ["outcome", "level", "$count"],
          color: "level",
        },
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: ["area", "outcome"],
          y: ["level", "$count"],
          color: "level",
        },
      },
    }),

  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
