import { data } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: Object.assign(data, {
        filter: (record) =>
          (record.Country == "Austria" ||
            record.Country == "Belgium" ||
            record.Country == "Cyprus" ||
            record.Country == "Denmark" ||
            record.Country == "Greece" ||
            record.Country == "Germany") &&
          record.Year >= 10,
      }),
      config: {
        channels: {
          x: "Year",
          y: ["Country", "Value 2 (+)"],
          color: "Country",
        },
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: "Joy factors",
        },
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: "Year",
          color: "Country",
        },
      },
    }),
];

export default testSteps;
