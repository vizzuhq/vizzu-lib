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
              title: "Plot, title and legend background",
              channels: {
                y: { set: ["Popularity", "Kinds"] },
                x: { set: "Genres" },
                label: { attach: "Popularity" },
              },
              color: { set: "Kinds" },
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: {
              title: {
                backgroundColor: "#A0A0A0",
              },
              plot: {
                backgroundColor: "#D2D2D2",
              },
              legend: {
                backgroundColor: "#808080",
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
              title: "Legend width",
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: {
              legend: {
                width: 50,
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
            style: {
              legend: {
                width: null,
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
              title: "Title padding",
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: {
              title: {
                paddingTop: 20,
                paddingBottom: 20,
                paddingLeft: 200,
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
            style: {
              title: {
                paddingTop: null,
                paddingBottom: null,
                paddingLeft: null,
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
              title: "Plot padding",
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: {
              plot: {
                paddingLeft: 100,
                paddingRight: 100,
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
            style: {
              plot: {
                paddingLeft: null,
                paddingRight: null,
              },
            },
          });
        },
      ],
    },
  ]);
});
