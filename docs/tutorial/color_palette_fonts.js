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
              title: "Color palette",
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
              plot: {
                marker: {
                  colorPalette: "#9355e8FF #123456FF #BDAF10FF",
                },
              },
            },
          });
        },
        (chart) => {
          console.log(chart.style); // eslint-disable-line no-console
          return chart;
        },
      ],
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: "Title font size",
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: {
              title: {
                fontSize: 50,
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
              title: "Title font size - back to default",
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: {
              title: {
                fontSize: null,
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
              title: "Setting all font sizes in one step",
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: {
              fontSize: 20,
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
              title: "Setting all style settings back to default",
            },
          });
        },
        (chart) => {
          return chart.animate({
            style: null,
          });
        },
      ],
    },
  ]);
});
