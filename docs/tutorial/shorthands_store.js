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
              title: "When only the config property is used",
              channels: {
                y: { set: ["Popularity", "Kinds"] },
                x: { set: "Genres" },
                color: { set: "Kinds" },
                label: { set: "Popularity" },
              },
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              align: "stretch",
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
              title: "Store function",
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
              title: "When just one series is used",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: { detach: "Kinds" },
                x: { attach: "Kinds" },
              },
              align: "none",
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
              title: "When you use set and no other channel options",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              channels: {
                y: ["Kinds", "Popularity"],
                x: "Genres",
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
              title: "You don't have to use the channel object",
            },
          });
        },
        (chart) => {
          return chart.animate({
            config: {
              y: "Kinds",
              x: ["Genres", "Popularity"],
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
              title: "You don't have to use the keyframe object",
            },
          });
        },
        (chart) => {
          return chart.animate([
            {
              y: ["Genres", "Popularity"],
              x: "Kinds",
            },
            {
              y: "Kinds",
              x: ["Genres", "Popularity"],
            },
          ]);
        },
      ],
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: "Shorthand for styles",
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: {
              "plot.xAxis.label.fontSize": "150%",
              "title.backgroundColor": "#A0A0A0",
            },
          });
        },
      ],
    },
    {
      anims: [
        (chart) => {
          return chart.animate(
            {
              style: null,
              config: {
                title: "Store function",
                align: "stretch",
                channels: {
                  y: { set: ["Popularity", "Kinds"] },
                  x: { set: "Genres" },
                  label: { attach: "Popularity" },
                },
                color: { set: "Kinds" },
              },
            },
            0
          );
        },
        (chart) => {
          return chart.animate({
            config: {
              title: "When you use set and no other channel options",
            },
          });
        },
        (chart) => {
          return chart.animate({
            channels: {
              // x: { attach: [ 'Kinds' ] },
              x: {
                attach: "Kinds",
              },
              // y: { detach: [ 'Kinds' ] },
              y: {
                detach: "Kinds",
              },
            },
            align: "none",
          });
        },
      ],
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            style: null,
            config: {
              title: "Store function",
              align: "stretch",
              channels: {
                y: { set: ["Popularity", "Kinds"] },
                x: { set: "Genres" },
                label: { attach: "Popularity" },
              },
              color: { set: "Kinds" },
            },
          });
        },
      ],
    },
  ]);
});
