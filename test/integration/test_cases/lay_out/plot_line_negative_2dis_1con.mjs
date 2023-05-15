import { data } from "../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          y: { attach: ["Value 5 (+/-)"], range: { min: "0%", max: "110%" } },
          x: { attach: ["Year"] },
          color: { attach: ["Country"] },
        },
        title: null,
        geometry: "line",
        legend: null,
      },
      style: {
        legend: {
          backgroundColor: "rgb(180,180,180 )",
        },
      },
    }),
];

export default testSteps;
