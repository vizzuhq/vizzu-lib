import { data } from "../../../test_data/infinite_data.mjs";

const testSteps = [
  (chart) =>
    chart.animate(
      {
        data: data,
        config: {
          channels: {
            x: { attach: ["Hor"], range: { max: "695" } },
            y: { attach: ["Ver"], range: { max: "375" } },
            color: { set: ["Joy factors"] },
          },
          align: "none",
          geometry: "circle",
          legend: null,
        },
        style: {
          plot: {
            marker: {
              circleMinRadius: 0.076,
              colorPalette: "#ee1b22FF #cecf07FF #01abf6FF #0ca749FF",
              label: {
                fontSize: 8,
              },
              guides: { color: "#ffffff00" },
            },
            xAxis: {
              color: "#ffffff00",
              title: { color: "#ffffff00" },
              label: { color: "#ffffff00" },
              interlacing: { color: "#ffffff00" },
              ticks: { color: "#ffffff00" },
            },
            yAxis: {
              color: "#ffffff00",
              title: { color: "#ffffff00" },
              label: { color: "#ffffff00" },
              interlacing: { color: "#ffffff00" },
              ticks: { color: "#ffffff00" },
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { detach: ["Hor"], range: { max: "119.17%" } },
            y: { detach: ["Ver"], range: { max: "127.86%" } },
            size: { set: ["Value 1"] },
          },
          title: "2",
        },
        style: {
          plot: {
            marker: {
              circleMinRadius: null,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            size: { set: ["Value 1", "Country code"] },
          },
          title: "3",
          //         legend: 'color'
        },
        style: {
          plot: {
            marker: {
              circleMinRadius: null,
              colorPalette: "#ee1b22FF #cecf07FF #01abf6FF #0ca749FF",
              label: {
                fontSize: 8,
              },
            },
            xAxis: {
              color: null,
              title: null,
              label: null,
              interlacing: null,
              ticks: null,
              guides: null,
            },
            yAxis: {
              color: null,
              title: null,
              label: null,
              interlacing: null,
              ticks: null,
              guides: null,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            label: { set: ["Country code"] },
          },
          title: "4",
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1"], range: { max: "110%" } },
            y: { set: ["Value 3"], range: { max: "110%" } },
            size: { set: ["Value 1"] },
            noop: { set: ["Country code"] },
          },
          title: "5",
        },
        style: {
          plot: {
            marker: {
              circleMinRadius: 0.001,
              circleMaxRadius: 0.04,
              label: {
                fontSize: null,
              },
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            y: { set: ["Joy factors"], range: { min: "-10%", max: "110%" } },
            label: { set: null },
          },
          title: "6",
          geometry: "rectangle",
          orientation: "vertical",
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 1,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            noop: { detach: ["Country code"] },
            x: { set: ["Country code"], range: { max: "100%" } },
            y: {
              set: ["Value 1", "Joy factors"],
              range: { min: "-10%", max: "110%" },
            },
          },
          title: "7",
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: null,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            y: { attach: ["Region"] },
            lightness: { set: ["Region"] },
          },
          title: "8",
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Region", "Country code"] },
            y: { set: ["Value 1", "Joy factors"] },
          },
          title: "9",
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Region", "Country code", "Joy factors"] },
            y: { set: ["Value 1"] },
          },
          title: "10",
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              set: ["Value 1", "Region", "Country code"],
              range: { max: "110%" },
            },
            y: { set: ["Joy factors"], range: { min: "-15%", max: "110%" } },
          },
          title: "11",
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              set: ["Value 1", "Region", "Country code"],
              range: { max: "133%" },
            },
            y: { set: ["Joy factors"], range: { min: "-30%", max: "100%" } },
          },
          title: "12",
          coordSystem: "polar",
          //           legend: 'lightness'
        },
        style: {
          plot: {
            xAxis: {
              title: { color: "#ffffff00" },
              label: { color: "#ffffff00" },
              ticks: { color: "#ffffff00" },
              interlacing: { color: "#ffffff00" },
            },
            yAxis: {
              color: "#ffffff00",
              title: { color: "#ffffff00" },
              ticks: { color: "#ffffff00" },
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1", "Country code"] },
            y: {
              set: ["Joy factors", "Region"],
              range: { min: "-10%", max: "100%" },
            },
          },
          title: "13",
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1"], range: { max: "110%" } },
            y: {
              set: ["Joy factors", "Region", "Country code"],
              range: { min: "-10%", max: "100%" },
            },
          },
          title: "14",
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              set: ["Value 1", "Joy factors", "Region", "Country code"],
              range: { max: "100%" },
            },
            y: { set: null, range: { min: "-200%", max: "100%" } },
            lightness: { set: null },
          },
          title: "15",
          //            legend: 'color'
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: null,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              set: ["Value 1", "Joy factors", "Region", "Country code"],
              range: { max: "100%" },
            },
            y: { set: null, range: { min: "0%", max: "100%" } },
          },
          title: "16",
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: null,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              set: ["Value 1", "Joy factors", "Region", "Country code"],
              range: { max: "100%" },
            },
            y: { set: ["Value 1"], range: { min: "-80%", max: "100%" } },
          },
          title: "17",
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: null,
              borderWidth: 0,
              borderOpacity: 1,
            },
            yAxis: {
              label: { color: "#ffffff00" },
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Region"], range: { max: "100%" } },
            y: {
              set: ["Value 1", "Country code"],
              range: { min: "-20%", max: "100%" },
            },
          },
          title: "18",
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 0.1,
              borderWidth: null,
              borderOpacity: null,
            },
            xAxis: {
              title: { color: null },
              label: { color: null },
              ticks: { color: null },
              interlacing: { color: null },
            },
            yAxis: {
              color: null,
              //                    title: { color: null },
              ticks: { color: null },
              //                    label: { color: null },
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors"], range: { max: "100%" } },
            y: {
              set: ["Value 1", "Region", "Country code"],
              range: { min: "0%", max: "100%" },
            },
          },
          title: "19",
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 0,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              set: ["Value 1", "Joy factors", "Region", "Country code"],
              range: { max: "100%" },
            },
            y: { set: null, range: { min: "-200%", max: "100%" } },
          },
          title: "20",
          coordSystem: "cartesian",
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              set: ["Value 1", "Joy factors", "Country code"],
              range: { max: "100%" },
            }, // regiont levesz
            y: { set: null, range: { min: "-200%", max: "100%" } },
          },
          title: "21",
          coordSystem: "cartesian",
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: null,
            },
          },
        },
      },
      "400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"], range: { max: "100%" } },
            y: { set: ["Value 1"], range: { min: "0%", max: "110%" } },
          },
          title: "22",
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 1,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"], range: { max: "100%" } },
            y: {
              set: ["Value 1", "Age group"],
              range: { min: "0%", max: "110%" },
            },
            color: { set: ["Age group"] },
          },
          title: "23",
          //           legend: 'lightness'
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 1,
              colorPalette: null,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"], range: { max: "100%" } },
            y: {
              set: ["Value 1", "Age group"],
              range: { min: "0%", max: "100%" },
            },
          },
          title: "24",
          split: true,
          //           legend: 'color'
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 1,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"], range: { max: "100%" } },
            y: {
              set: ["Value 1", "Age group"],
              range: { min: "0%", max: "100%" },
            },
          },
          title: "25",
          geometry: "area",
          align: "stretch",
          split: false,
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 0,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"], range: { max: "100%" } },
            y: {
              set: ["Value 1", "Age group"],
              range: { min: "0%", max: "100%" },
            },
          },
          title: "26",
          geometry: "area",
          align: "center",
          split: true,
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 0,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"], range: { max: "100%" } },
            y: { set: ["Value 1"], range: { min: "0%", max: "100%" } },
            noop: { set: ["Age group"] },
            color: { set: ["Joy factors"] },
          },
          title: "27",
          geometry: "circle",
          align: "center",
          split: true,
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 0,
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors"], range: { min: "0%", max: "100%" } },
            y: { set: ["Value 1"], range: { min: "0%", max: "100%" } },
            color: { set: ["Joy factors"] },
            lightness: { set: ["Value 3"] },
            noop: { set: ["Country code", "Age group"] },
            size: { set: ["Value 2"] },
          },
          title: "28",
          geometry: "circle",
          align: "min",
          orientation: "horizontal",
          split: false,
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: null,
              circleMinRadius: 0.001,
              circleMaxRadius: 0.025,
              colorPalette: "#ee1b22FF #cecf07FF #01abf6FF #0ca749FF",
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors"], range: { min: "0%", max: "100%" } },
            y: { set: ["Value 1"], range: { min: "0%", max: "100%" } },
            color: { set: ["Joy factors"] },
            lightness: { set: ["Value 3"] },
            noop: { set: ["Country code", "Age group"] },
            size: { set: ["Value 2"] },
          },
          title: "29",
          geometry: "circle",
          align: "min",
          orientation: "horizontal",
          split: false,
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: null },
            y: { set: null },
            color: { set: ["Joy factors"] },
            noop: { set: ["Age group"] },
            size: { set: ["Value 1", "Country code"] },
          },
          title: "30",
          geometry: "circle",
          align: "none",
          split: false,
        },
        style: {
          plot: {
            marker: {
              label: {
                fontSize: 8,
              },
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: null },
            y: { set: null },
            color: { set: ["Joy factors"] },
            size: { set: ["Value 1", "Country code", "Age group"] },
          },
          title: "31",
          geometry: "circle",
          align: "none",
          split: false,
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: null },
            y: { set: null },
            color: { set: ["Joy factors"] },
            label: { set: ["Joy factors"] },
            lightness: { set: null },
            noop: { set: null },
            size: { set: ["Value 1"] },
          },
          title: "32",
          geometry: "circle",
          align: "none",
          split: false,
        },
        style: {
          plot: {
            marker: {
              label: {
                fontSize: null,
              },
            },
          },
        },
      },
      "1500ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { attach: ["Hor"], range: { max: "695" } },
            y: { attach: ["Ver"], range: { max: "375" } },
            color: { set: ["Joy factors"] },
            label: { set: null },
          },
          title: null,
          align: "none",
          geometry: "circle",
          legend: null,
        },
        style: {
          plot: {
            marker: {
              circleMinRadius: 0.076,
              label: {
                fontSize: 8,
              },
              guides: { color: "#ffffff00" },
            },
            xAxis: {
              color: "#ffffff00",
              title: { color: "#ffffff00" },
              label: { color: "#ffffff00" },
              interlacing: { color: "#ffffff00" },
              ticks: { color: "#ffffff00" },
            },
            yAxis: {
              color: "#ffffff00",
              title: { color: "#ffffff00" },
              label: { color: "#ffffff00" },
              interlacing: { color: "#ffffff00" },
              ticks: { color: "#ffffff00" },
            },
          },
        },
      },
      "1500ms"
    ),
];

export default testSteps;
