import { data } from "../../test_data/chart_types_eu.mjs";

/** Title should fade in beside the chart, not after. */

const testSteps = [
  (chart) => {
    let data = {
      series: [
        { name: "Foo", values: ["Alice", "Bob", "Ted"] },
        { name: "Bar", values: [15, 32, 12] },
        { name: "Baz", values: [5, 3, 2] },
      ],
    };

    return chart.animate({ data });
  },
  (chart) =>
    chart.animate({
      x: "Foo",
      y: "Bar",
      title: "Test",
    }),
];

export default testSteps;
