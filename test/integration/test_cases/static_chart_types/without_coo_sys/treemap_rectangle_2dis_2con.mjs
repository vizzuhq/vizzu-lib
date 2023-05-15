import { data } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          color: "Joy factors",
          size: ["Value 2 (+)", "Country_code"],
          label: "Country_code",
          lightness: "Value 2 (+)",
        },
        title: "Treemap (Stacked+2conti)",
      },
    }),
];

export default testSteps;
