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
          x: { set: ["Year"] },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: ["Country"] },
        },
        title: "Stacked Column Chart",
        geometry: "rectangle",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 2 (+)", "Year"] },
            y: { set: ["Country"] },
            color: { set: ["Country"] },
          },
          title: "Radial Chart",
          geometry: "rectangle",
          coordSystem: "polar",
          orientation: "horizontal",
          split: false,
        },
      },
      {
        easing: "cubic-bezier(0.65,0,0.65,1)",
        coordSystem: {
          delay: 0,
          duration: 1,
        },
        geometry: {
          delay: 0.5,
          duration: 0.5,
          easing: "linear",
        },
        x: {
          delay: 0.5,
          duration: 0.5,
          //                easing: 'ease-out'
        },
        y: {
          delay: 0,
          duration: 0.5,
          //                easing: 'cubic-bezier(.39,0,.35,.99)'
        },
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Year"] },
            y: { set: ["Country", "Value 2 (+)"] },
            color: { set: ["Country"] },
          },
          title: "Stacked Column Chart",
          coordSystem: "cartesian",
          geometry: "rectangle",
          legend: null,
        },
      },
      {
        easing: "cubic-bezier(0.65,0,0.65,1)",
        coordSystem: {
          delay: 0,
          duration: 1,
        },
        geometry: {
          delay: 0,
          duration: 0.5,
          easing: "linear",
        },
        x: {
          delay: 0,
          duration: 0.5,
          //                easing: 'ease-out'
        },
        y: {
          delay: 0.5,
          duration: 0.5,
          //                easing: 'cubic-bezier(.39,0,.35,.99)'
        },
      }
    ),
];

export default testSteps;
