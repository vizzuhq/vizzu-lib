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
              title: "Switch the orientation = arrange by other axis",
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
                y: {
                  detach: "Popularity",
                },
                x: {
                  attach: "Popularity",
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
              title: "Split stacked values = show side-by-side",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              split: true,
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
              title: "Merge",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              split: false,
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
              title: "Aggregate",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                x: {
                  detach: "Genres",
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
              title: "Polar coordinates",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: {
                  range: {
                    min: "-30%",
                  },
                },
              },
              coordSystem: "polar",
            },
          });
        },
      ],
    },
  ]);
});
