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
          record.Country == "Germany" ||
          record.Country == "Denmark" ||
          record.Country == "Estonia" ||
          record.Country == "Greece" ||
          record.Country == "Spain" ||
          record.Country == "Finland" ||
          record.Country == "France" ||
          record.Country == "Croatia" ||
          record.Country == "Hungary",
      }),
      config: {
        channels: {
          size: { set: ["Year", "Value 2 (+)"] },
          color: { set: ["Country"] },
          noop: { set: ["Year"] },
        },
        title: "Treemap",
        geometry: "rectangle",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Year"] },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: ["Country"] },
        },
        title: "Stacked Column Chart",
        geometry: "rectangle",
        //            orientation: 'horizontal',
        split: false,
      },
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: null },
          y: { set: null },
          size: { set: ["Year", "Value 2 (+)"] },
          color: { set: ["Country"] },
          noop: { set: ["Year"] },
        },
        title: "Treemap",
        geometry: "rectangle",
        legend: null,
      },
    }),
];

export default testSteps;
