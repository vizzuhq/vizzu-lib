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
          x: { set: ["Year", "Value 3 (+)"] },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: "Country" },
        },
        title: "Mekko Chart",
        orientation: "horizontal",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Year", "Value 3 (+)"] },
          y: { set: ["Country", "Value 2 (+)"] },
          color: null,
        },
        title: "Remove Color & Stretch & axisLabel off",
        align: "stretch",
      },
      style: {
        plot: {
          xAxis: { label: { color: "rgb(255,255,255)" } },
        },
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Value 3 (+)"] },
            y: { set: "Value 2 (+)" },
            color: null,
          },
          title: "Change Disc",
        },
      },
      {
        duration: 0,
      }
    ),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Joy factors", "Value 3 (+)"] },
          y: { set: "Value 2 (+)" },
          color: { set: "Joy factors" },
        },
        title: "Add new Disc Color & Stretch off & axisLabel on",
        align: "none",
      },
      style: {
        plot: {
          xAxis: { label: { color: null } },
        },
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: null,
          y: null,
          color: { set: "Joy factors" },
          lightness: { set: "Value 3 (+)" },
          size: { set: "Value 2 (+)" },
        },
        title: "add new Disc Color & Change CoordSys & axisLabel on",
      },
      style: {
        plot: {
          xAxis: { label: { color: null } },
        },
      },
    }),
];

export default testSteps;
