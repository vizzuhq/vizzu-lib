import { data } from "../../../../../test_data/chart_types_eu.mjs";

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
          record.Country == "Spain" ||
          record.Country == "Finland" ||
          record.Country == "France" ||
          record.Country == "Croatia" ||
          record.Country == "Hungary",
      }),
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 2 (+)" },
          color: { set: "Country" },
        },
        title: "Lollipop Chart",
        geometry: "circle",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: null,
          y: null,
          color: "Joy factors",
          size: ["Year", "Value 1 (+)"],
        },
        title: "Stack new Disc, Change CoordSys",
      },
    }),
];

export default testSteps;
