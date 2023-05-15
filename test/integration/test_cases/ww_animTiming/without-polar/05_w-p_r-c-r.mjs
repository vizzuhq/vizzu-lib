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
    chart.animate(
      {
        config: {
          channels: {
            noop: { set: ["Year"] },
            size: { set: null },
            x: { set: ["Value 5 (+/-)"] },
            y: { set: ["Value 2 (+)"] },
            color: { set: ["Country"] },
          },
          title: "Dotplot vs. Scatter plot",
          geometry: "circle",
          coordSystem: "polar",
          //            orientation: 'horizontal',
          split: false,
        },
      },
      {
        coordSystem: {
          delay: 0,
          duration: 1,
        },
        geometry: {
          delay: 0,
          duration: 0.5,
          //                easing: 'linear'
        },
        x: {
          delay: 0.25,
          duration: 0.75,
          //               easing: 'ease-in'
        },
        y: {
          delay: 0.25,
          duration: 0.75,
          //                easing: 'cubic-bezier(.39,0,.35,.99)'
        },
      }
    ),
  (chart) =>
    chart.animate(
      {
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
          coordSystem: "cartesian",
          legend: null,
        },
      },
      {
        coordSystem: {
          delay: 0,
          duration: 1,
        },
        geometry: {
          delay: 0.5,
          duration: 0.5,
          //                easing: 'linear'
        },
        x: {
          delay: 0,
          duration: 0.75,
          //               easing: 'ease-in'
        },
        y: {
          delay: 0,
          duration: 0.75,
          //                easing: 'cubic-bezier(.39,0,.35,.99)'
        },
      }
    ),
];

export default testSteps;
