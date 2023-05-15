import { data_14 } from "../../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) =>
    chart.animate({
      data: data_14,
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 2 (+)" },
          color: { set: "Country" },
        },
        title: "Line Chart",
        geometry: "line",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 2 (+)" },
          color: null,
          size: { set: "Country" },
        },
        title: "Stack Disc & Remove Color",
      },
    }),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 1 (+)" },
          color: null,
          size: { set: "Country" },
        },
        title: "Change Conti",
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: "Year" },
            y: { set: "Value 1 (+)" },
            color: null,
            size: { set: "Joy factors" },
          },
          title: "Change Disc",
        },
      },
      {
        duration: 0,
      }
    ),

  (chart) =>
    chart.animate({
      config: {
        channels: {
          x: { set: "Year" },
          y: { set: "Value 1 (+)" },
          color: { set: "Joy factors" },
          size: null,
        },
        title: "Group new Disc & Add new Disc Color",
      },
    }),

  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: null,
            y: null,
            noop: { set: "Year" },
            size: { set: ["Year", "Value 1 (+)"] },
          },
          title: "Stack new Disc & Change Geoms & CoordSys",
          geometry: "rectangle",
        },
      },
      {
        geometry: {
          delay: 0.5,
          duration: 1.5,
        },
        x: {
          delay: 1,
        },
      }
    ),
];

export default testSteps;
