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
              channels: {
                y: { set: "Popularity" },
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
              channels: {
                x: { set: null },
                y: { set: ["Genres", "Popularity"] },
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
              channels: {
                y: { detach: "Popularity" },
                x: { attach: "Popularity" },
              },
            },
          });
        },
      ],
    },
    {
      anims: [
        (chart) => {
          console.log(chart.config); // eslint-disable-line no-console
          return chart;
        },
        (chart) => {
          return chart.animate({ title: "My first chart" });
        },
      ],
    },
    {
      anims: [
        (chart) => {
          chart.feature("tooltip", true);
          return chart;
        },
      ],
    },
  ]);
});
