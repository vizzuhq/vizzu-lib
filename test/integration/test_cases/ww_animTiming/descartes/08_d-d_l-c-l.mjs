import { data } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: Object.assign(data, {
        filter: (record) =>
          record.Country == "Austria" ||
          record.Country == "Belgium" ||
          record.Country == "Bulgaria" ||
          record.Country == "Cyprus" ||
          record.Country == "Czechia" ||
          record.Country == "Denmark" ||
          record.Country == "Estonia" ||
          record.Country == "Greece" ||
          record.Country == "Germany" ||
          //               record.Country == 'Spain' ||
          //               record.Country == 'Finland' ||
          //               record.Country == 'France' ||
          //               record.Country == 'Croatia' ||
          record.Country == "Hungary",
      }),
      config: {
        channels: {
          x: { set: ["Year"] },
          y: { set: ["Value 2 (+)"] },
          color: { set: ["Country"] },
        },
        title: "Stacked Area Chart",
        geometry: "line",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Year", "Value 5 (+/-)"] },
            y: { set: ["Value 2 (+)", "Country"] },
            color: { set: ["Country"] },
          },
          title: "Scatter plot",
          geometry: "circle",
          //            orientation: 'horizontal',
          split: false,
        },
      },
      {
        geometry: {
          delay: 0,
          duration: 1,
        },
        x: {
          delay: 0.5,
        },
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Year"] },
            y: { set: ["Value 2 (+)"] },
            color: { set: ["Country"] },
          },
          title: "Stacked Area Chart",
          geometry: "line",
          legend: null,
        },
      },
      {
        geometry: {
          delay: 1,
          duration: 1,
        },
        y: {
          delay: 0.5,
        },
      }
    ),
];

export default testSteps;
