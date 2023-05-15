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
          record.Country == "Denmark",
      }),
      config: {
        channels: {
          y: { set: "Country", range: { min: "-50%" } },
          x: "Value 1 (+)",
          label: "Value 1 (+)",
        },
        title: "Radial Bar Chart",
        coordSystem: "polar",
      },
    }),
  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
