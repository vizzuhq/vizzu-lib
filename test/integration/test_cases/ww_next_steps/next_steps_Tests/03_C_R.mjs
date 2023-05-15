import { data_14 } from "../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_14,
      config: {
        channels: {
          x: { set: ["Year", "Country"] },
          y: { set: ["Value 2 (+)"] },
          color: { set: "Country" },
        },
        title: "1 Groupped Column",
        geometry: "rectangle",
        orientation: "horizontal",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: {
            set: ["Country", "Value 2 (+)"],
            range: { min: "0%", max: "110%" },
          },
          color: { set: "Country" },
        },
        title: "2 Stacked Column / Sum(?) sub-elements",
        geometry: "rectangle",
        orientation: "horizontal",
        split: false,
        legend: "color",
      },
    }),
  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: ["Year", "Country"] },
          y: { set: ["Value 2 (+)"] },
          color: { set: "Country" },
        },
        title: "3 Groupped Column / Comparison(?), Group(?)",
        geometry: "rectangle",
        orientation: "horizontal",
        legend: null,
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: null },
            y: {
              set: ["Country", "Value 2 (+)"],
              range: { min: "0%", max: "110%" },
            },
            color: { set: "Country" },
            label: { set: ["Value 2 (+)"] },
          },
          title:
            "5 Stacked Column / Aggregate(?), Sum(?), - Discrete(?) elements",
          geometry: "rectangle",
          orientation: "horizontal",
          split: false,
        },
      },
      {
        delay: 0,
        duration: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: ["Year", "Country"],
            y: ["Value 2 (+)"],
            color: "Country",
            label: null,
          },
          title:
            "7 Groupped Column / Drill down(?), + Discrete(?), Groupped(?)",
          geometry: "rectangle",
          orientation: "horizontal",
          legend: null,
        },
      },
      {
        delay: 0,
        duration: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: {
              set: ["Country", "Value 2 (+)"],
              range: { min: "0%", max: "100%" },
            },
            color: { set: "Country" },
          },
          title: "8 Stacked Percentage Column / Ratio%(?)",
          align: "stretch",
          legend: "color",
        },
      },
      {
        delay: 0,
        duration: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Year", "Country"] },
            y: { set: "Value 2 (+)", range: { min: "0%", max: "110%" } },
            color: { set: "Country" },
          },
          title: "9 Groupped Column / Comparison(?), Group(?)",
          align: "min",
        },
      },
      {
        delay: 0,
        duration: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: null },
            y: { set: "Value 2 (+)", range: { min: "0%", max: "110%" } },
            color: { set: null },
            label: { set: "Value 2 (+)" },
          },
          title: "11 Column / Total(?) Aggregate(?) - Discrete(?)",
          align: "min",
          legend: null,
        },
      },
      {
        delay: 0,
        duration: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Year", "Country"] },
            y: { set: "Value 2 (+)", range: { min: "0%", max: "110%" } },
            color: { set: "Country" },
            label: null,
          },
          title: "13 Groupped Column / Comparison(?), Group(?)",
          align: "min",
          legend: "color",
        },
      },
      {
        delay: 0,
        duration: 1,
      }
    ),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Country", "Value 2 (+)"] },
            y: { set: null, range: { min: "0%", max: "100%" } },
            color: { set: "Country" },
            label: { set: "Value 2 (+)" },
          },
          title: "15 Stacked Bar / Sum(?)",
          align: "min",
        },
        style: {
          plot: {
            marker: {
              label: {
                fontSize: 9,
              },
            },
          },
        },
      },
      {
        delay: 0,
        duration: 1,
      }
    ),

  (chart) => {
    chart.feature("tooltip", true);
    return chart;
  },
];

export default testSteps;
