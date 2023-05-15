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
          y: { set: "Value 2 (+)" },
          color: null,
          noop: { set: "Country" },
          size: { set: "Year" },
        },
        title: "Stack Discrete & Remove Color",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: null,
          y: { set: "Value 2 (+)" },
          color: null,
          noop: { set: "Country" },
          size: { set: "Joy factors" },
        },
        title: "Change Disc",
      },
    }),
  /*
    chart => chart.animate({
        config: {
            channels: {
                x: null,
                y: { set: 'Value 3 (+)' },
                color: null,
                noop: { set: 'Country' },
                size: { set: 'Joy factors' }
            },
            title: 'Change Conti'
        }
    }
    ),
*/
  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Joy factors" },
          y: { set: "Value 3 (+)" },
          color: { set: "Joy factors" },
          noop: { set: "Country" },
          size: null,
        },
        title: "Group new Discrete & Add new Disc Color",
      },
    }),
];

export default testSteps;
