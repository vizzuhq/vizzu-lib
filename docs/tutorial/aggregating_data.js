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
              title: "Sum of all Popularity Values",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { set: "Popularity" },
              },
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              title: "Sum of Popularity by Genre",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                x: { set: "Genres" },
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
              title: "Minimum of Popularity by Genre",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { set: "min(Popularity)" },
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
              title: "Maximum of Popularity by Genre",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { set: "max(Popularity)" },
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
              title: "Mean of Popularity by Genre",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { set: "mean(Popularity)" },
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
              title: "Count of items by Genre",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { set: "count()" },
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
              title: "Distinct Kinds by Genre",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { set: "distinct(Kinds)" },
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
              title: "Sum of Popularity by Genre",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { set: "sum(Popularity)" },
              },
            },
          });
        },
      ],
    },
  ]);
});
