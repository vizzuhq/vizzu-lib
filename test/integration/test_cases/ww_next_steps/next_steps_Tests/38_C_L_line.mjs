import { data_6 } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate(
      {
        data: data_6,
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: "Value 2 (+)" },
            color: { set: "Country" },
          },
          title: "1 Line",
          geometry: "line",
        },
        style: {
          title: {
            fontSize: "2em",
          },
        },
      },
      {
        delay: 0,
      }
    ),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: ["Value 2 (+)", "Country"] },
          color: { set: "Country" },
        },
        title: "2 Stacked Area / Sum(?), Stack(?)",
        geometry: "area",
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: "Value 2 (+)" },
            color: { set: "Country" },
          },
          title: "3 Line / Comparison(?), Components(?)",
          geometry: "line",
          split: false,
        },
      },
      {
        delay: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: ["Value 2 (+)", "Country"] },
            color: { set: "Country" },
          },
          title:
            "4 Trellis Area / Trellis(?), Components(?), Part-to-whole(?) (sort???)",
          geometry: "area",
          split: true,
        },
      },
      {
        delay: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: "Value 2 (+)" },
            color: { set: "Country" },
          },
          title: "5 Line / Comparison(?)",
          geometry: "line",
          split: false,
        },
      },
      {
        delay: 1,
      }
    ),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: ["Value 2 (+)", "Country"] },
          color: { set: "Country" },
        },
        title: "6 Stacked Area / Ratio(?)",
        geometry: "area",
        align: "stretch",
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: "Value 2 (+)" },
            color: { set: "Country" },
          },
          title: "7 Line / Comparison(?), Components(?)",
          geometry: "line",
          align: "min",
        },
      },
      {
        delay: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: "Value 2 (+)" },
            size: { set: "Country" },
            color: { set: null },
          },
          title: "8 Line / Aggregate(?), - Discrete(?)",
          geometry: "line",
          align: "min",
        },
      },
      {
        delay: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: "Value 2 (+)" },
            color: { set: "Country" },
            size: { set: null },
          },
          title: "9 Line / Drill down(?), + Discrete(?)",
          geometry: "line",
          align: "min",
        },
      },
      {
        delay: 1,
      }
    ),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Year", "Value 2 (+)"] },
          y: { set: "Country" },
          color: { set: null },
        },
        title: "10 Bar / Sum(?)",
        geometry: "rectangle",
        align: "min",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 2 (+)"] },
            y: { set: "Country" },
            color: { set: null },
            label: { set: "Value 2 (+)" },
          },
          title: "11 Bar / Sum(?)",
          geometry: "rectangle",
          align: "min",
          legend: null,
        },
      },
      {
        delay: 0,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Year", "Value 2 (+)"] },
            y: { set: "Country" },
            color: { set: null },
            label: { set: null },
          },
          title: "12 Bar / Sum(?)",
          geometry: "rectangle",
          align: "min",
          legend: null,
        },
      },
      {
        delay: 1,
      }
    ),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 2 (+)" },
          color: { set: "Country" },
          size: { set: null },
        },
        title: "13 Line / Drill down timeseries(?), + Discrete(?)",
        geometry: "line",
        align: "min",
        legend: "color",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: "Country" },
        },
        title: "14 Stacked Column / Sum(?), Stack(?)",
        geometry: "rectangle",
        align: "min",
        legend: "color",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 2 (+)" },
          color: { set: "Country" },
        },
        title: "15 Line / Comparison(?), + Components(?)",
        geometry: "line",
        align: "min",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: ["Country", "Value 2 (+)"] },
          color: { set: "Country" },
        },
        title: "16 Line / Comparison(?), + Components(?)",
        geometry: "line",
        orientation: "vertical",
        split: true,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Country" },
          color: { set: null },
          lightness: { set: "Value 2 (+)" },
        },
        title: "17 Heatmap / (?), (?), (sort???)",
        geometry: "rectangle",
        legend: null,
        split: false,
      },
      style: {
        plot: {
          marker: {
            rectangleSpacing: 0,
          },
        },
      },
    }),
];

export default testSteps;
