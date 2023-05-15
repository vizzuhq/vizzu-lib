import { data } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          y: { attach: ["Value 5 (+/-)"], range: { min: "0%", max: "110%" } },
          x: { attach: ["Value 6 (+/-)"], range: { min: "0%", max: "110%" } },
          lightness: { attach: ["Country"] },
          size: { attach: ["Value 4 (+/-)"] },
          label: { attach: ["Value 5 (+/-)"] },
        },
        title: "Size Continuous + Discrete.",
        legend: "size",
        geometry: "circle",
      },
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          size: { attach: ["Country_code"] },
        },
      },
    }),
];

export default testSteps;
