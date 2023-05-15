import { data } from "../../../test_data/chart_types_eu.mjs";

data.filter = (record) =>
  record.Country == "Bulgaria" ||
  record.Country == "Germany" ||
  record.Country == "Malta" ||
  record.Country == "Lithuania";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data,
      config: {
        channels: {
          y: ["Joy factors", "Value 2 (+)"],
          x: ["Country", "Value 2 (+)"],
          color: "Joy factors",
          label: ["Value 2 (+)", "Country"],
        },
        title: "Stacked Mekko chart",
      },
    }),
];

export default testSteps;
