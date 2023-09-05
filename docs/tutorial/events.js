const dataLoaded = import("../assets/data/music_data.js");
const mdChartLoaded = import("../assets/javascripts/mdchart.js");

Promise.all([dataLoaded, mdChartLoaded]).then((results) => {
  const data = results[0].default;
  const MdChart = results[1].default;
  const mdchart = new MdChart(data, "./vizzu.js", "tutorial");

  const clickHandler = (event) => {
    alert(JSON.stringify(event.data)); // eslint-disable-line no-alert
  };

  const labelDrawHandler = (event) => {
    event.renderingContext.fillStyle =
      event.data.text === "Jazz" ? "red" : "gray";
  };

  const logoDrawHandler = (event) => {
    event.preventDefault();
  };

  const image = new Image();
  image.src =
    "data:image/gif;base64,R0lGODlhAwACAPIAAJLf6q/i7M/r8un0+PT6+/n8/QAAAAAAACH5BAQAAAAALAAAAAADAAIAAAMEWBMkkAA7";

  const backgroundImageHandler = (event) => {
    event.renderingContext.drawImage(
      image,
      0,
      0,
      event.data.rect.size.x,
      event.data.rect.size.y
    );
    event.preventDefault();
  };

  mdchart.create([
    {
      anims: [
        (chart) => {
          try {
            chart.off("click", clickHandler);
          } catch (error) {
            if (!error.toString().includes("unknown event handler")) {
              throw error;
            }
          }
          return chart.animate({
            config: {
              channels: {
                y: { set: ["Popularity", "Kinds"] },
                x: { set: "Genres" },
                label: { attach: "Popularity" },
              },
              color: { set: "Kinds" },
              title: "Click event added to markers",
            },
          });
        },
        (chart) => {
          chart.on("click", clickHandler);
          chart.render.updateFrame(true);
          return chart;
        },
      ],
    },
    {
      anims: [
        (chart) => {
          try {
            chart.off("plot-axis-label-draw", labelDrawHandler);
          } catch (error) {
            if (!error.toString().includes("unknown event handler")) {
              throw error;
            }
          }
          return chart.animate({
            config: {
              title: "Changing the canvas context before label draw",
            },
          });
        },
        (chart) => {
          chart.on("plot-axis-label-draw", labelDrawHandler);
          chart.render.updateFrame(true);
          return chart;
        },
      ],
    },
    {
      anims: [
        (chart) => {
          try {
            chart.off("logo-draw", logoDrawHandler);
          } catch (error) {
            if (!error.toString().includes("unknown event handler")) {
              throw error;
            }
          }
          return chart.animate({
            config: {
              title: "Prevent default behavior",
            },
          });
        },
        (chart) => {
          chart.on("logo-draw", logoDrawHandler);
          chart.render.updateFrame(true);
          return chart;
        },
      ],
    },
    {
      anims: [
        (chart) => {
          try {
            chart.off("background-draw", backgroundImageHandler);
          } catch (error) {
            if (!error.toString().includes("unknown event handler")) {
              throw error;
            }
          }
          return chart.animate({
            config: {
              title: "Add background image",
            },
          });
        },
        (chart) => {
          const registerHandler = () => {
            chart.on("background-draw", backgroundImageHandler);
            chart.render.updateFrame(true);
          };
          if (!image.complete) image.onload = registerHandler;
          else registerHandler();
          return chart;
        },
      ],
    },
  ]);
});
