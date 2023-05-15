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
          x: { set: "Value 2 (+)" },
          y: { set: "Country" },
          color: { set: "Country" },
        },
        title: "Bar",
        geometry: "rectangle",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Value 3 (+)" },
            y: { set: "Country" },
            color: { set: "Country" },
          },
          title: "Bar",
          geometry: "rectangle",
          legend: null,
        },
      },
      {
        duration: 1.5,
        easing: "cubic-bezier(.39,.14,.02,.99)",
        x: {
          easing: "ease-in",
          delay: 0.25,
          duration: 0.5,
        },
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Value 2 (+)" },
            y: { set: "Country" },
            color: { set: "Country" },
          },
          title: "Bar",
          geometry: "rectangle",
          legend: null,
        },
      },
      {
        duration: 1.5,
        easing: "cubic-bezier(.39,.0,.02,.99)",
        x: {
          easing: "cubic-bezier(.68,0,.32,.99)",
          delay: 0,
          duration: 0.5,
        },
      }
    ),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: null },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: "Country" },
        },
        title: "Column",
        geometry: "rectangle",
        legend: null,
      },
    }),
];

export default testSteps;
