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
        geometry: "rectangle",
        orientation: "horizontal",
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Year", "Value 3 (+)"] },
            y: { set: ["Country", "Value 2 (+)"] },
            color: { set: "Country" },
          },
          title: "Stretch & axisLabel off",
          align: "stretch",
        },
        style: {
          plot: {
            xAxis: { label: { color: "rgb(255,255,255)" } },
          },
        },
      },
      {
        easing: "cubic-bezier(.39,0,.35,.99)",
        y: {
          delay: 0,
        },
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Value 3 (+)"] },
            y: { set: ["Country", "Value 2 (+)"] },
            color: { set: "Country" },
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
          y: { set: ["Country", "Value 1 (+)"] },
          color: { set: "Country" },
        },
        title: "Change Conti",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Joy factors", "Value 3 (+)"] },
          y: { set: ["Country", "Value 1 (+)"] },
          color: { set: "Country" },
        },
        title: "Stretch off & axisLabel on",
        orientation: "horizontal",
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
          color: { set: "Country" },
          lightness: { set: "Value 3 (+)" },
          size: { set: ["Joy factors", "Value 1 (+)"] },
        },
        title: "Change CoordSys",
      },
    }),
];

export default testSteps;
