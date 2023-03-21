const dataLoaded = import("../assets/data/music_data.js");
const mdChartLoaded = import("../assets/javascripts/mdchart.js");

Promise.all([dataLoaded, mdChartLoaded]).then((results) => {
  const data = results[0].default;
  const MdChart = results[1].default;
  const mdchart = new MdChart(data, "./vizzu.js", "tutorial");

  mdchart.create([
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: "Aggregate",
              channels: {
                y: { set: ["Popularity", "Kinds"] },
                x: { set: "Genres" },
                label: { attach: "Popularity" },
              },
              color: { attach: "Kinds" },
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                x: {
                  set: null,
                },
              },
            },
          });
        },
      ],
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: "Changing dimensions",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { detach: ["Kinds"] },
                x: { set: ["Genres"] },
                color: { set: null },
              },
            },
          });
        },
      ],
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: "Changing dimensions by drilling down",
            },
          });
        },
        (chart) => {
          return chart.animate(
            {
              config: {
                channels: {
                  x: { detach: ["Genres"], attach: ["Kinds"] },
                },
              },
            },
            { regroupStrategy: "drilldown" }
          );
        },
      ],
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: "Changing dimensions with fading",
            },
          });
        },
        (chart) => {
          return chart.animate(
            {
              config: {
                channels: {
                  x: { detach: ["Kinds"], attach: ["Genres"] },
                },
              },
            },
            { regroupStrategy: "fade" }
          );
        },
      ],
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: "Drill-down",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { attach: ["Kinds"] },
                color: { set: ["Kinds"] },
              },
            },
          });
        },
      ],
    },
  ]);
});
