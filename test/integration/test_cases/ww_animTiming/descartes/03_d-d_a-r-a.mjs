import { data_14 } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_14,
      config: {
        channels: {
          x: { set: ["Year"] },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: ["Country"] },
        },
        title: "Stacked Area Chart",
        geometry: "area",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Year"] },
            y: { set: ["Value 2 (+)", "Country"] },
            color: { set: ["Country"] },
          },
          title: "Bar Chart",
          geometry: "rectangle",
          //            orientation: 'horizontal',
          split: false,
        },
      },
      {
        geometry: {
          delay: 0.7,
          duration: 0.5,
        },
        x: {
          delay: 1,
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
          title: "Stacked Area Chart",
          geometry: "area",
          legend: null,
        },
      },
      {
        geometry: {
          delay: 0,
          duration: 0.5,
        },
        x: {
          delay: 1,
        },
      }
    ),
];

export default testSteps;
