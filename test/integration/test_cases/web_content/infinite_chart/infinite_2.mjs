import { data } from "../../../test_data/infinite_data.mjs";
let step = 0;
let targetstep = -1;
const testSteps = [
  (chart) =>
    chart.animate(
      {
        data: data,
        config: {
          channels: {
            x: {
              attach: ["Hor"],
              range: { max: "688" },
            },
            y: {
              attach: ["Ver"],
              range: { max: "375" },
            },
            color: { set: ["Joy factors"] },
          },
          align: "none",
          geometry: "circle",
          legend: null,
        },
        style: {
          plot: {
            paddingTop: "16.6%",
            paddingLeft: "22.85%",
            paddingRight: "22.28%",
            paddingBottom: "19.2%",
            marker: {
              circleMinRadius: 0.076,
              colorPalette: "#ee1b22FF #cecf07FF #01abf6FF #0ca749FF",
              fillOpacity: 1,
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
          logo: {
            filter: "none",
            width: "151%",
            paddingRight: "20.4%",
            paddingBottom: "45.8%",
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          //            title: 'Disappears the logo'
        },
        style: {
          logo: {
            filter: "opacity(0)",
          },
          plot: {
            marker: {
              fillOpacity: null,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              detach: ["Hor"],
              range: { max: "119.17%" },
            },
            y: {
              detach: ["Ver"],
              range: { max: "127.86%" },
            },
            size: { set: ["Value 1"] },
          },
          //            title: '2'
        },
        style: {
          plot: {
            marker: {
              circleMinRadius: null,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            size: { set: ["Value 1", "Country code"] },
          },
          //            title: '3'
        },
        style: {
          plot: {
            marker: {
              colorPalette: "#ee1b22FF #cecf07FF #01abf6FF #0ca749FF",
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
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: {
              set: ["Value 1"],
              range: { max: "110%" },
            },
            y: {
              set: ["Value 3"],
              range: { max: "110%" },
            },
            size: { set: ["Value 1"] },
            noop: { set: ["Country code"] },
          },
          //            title: '4'
        },
        style: {
          plot: {
            marker: {
              circleMinRadius: 0.001,
              circleMaxRadius: 0.04,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            y: {
              set: ["Joy factors"],
              range: { min: "-10%" },
            },
          },
          //            title: '5',
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
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            noop: { detach: ["Country code"] },
            x: {
              set: ["Country code"],
              range: { max: "100%" },
            },
            y: { set: ["Value 1", "Joy factors"] },
          },
          //            title: '6'
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: null,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            y: { attach: ["Region"] },
            lightness: { set: ["Region"] },
          },
          //            title: '7'
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Region", "Country code"] },
            y: { set: ["Value 1", "Joy factors"] },
          },
          //            title: '8'
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Region", "Country code", "Joy factors"] },
            y: { set: ["Value 1"] },
          },
          //                title: '9'
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
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
            y: {
              set: ["Joy factors"],
              range: { min: "-15%" },
            },
          },
          //            title: '10'
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
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
            y: {
              set: ["Joy factors"],
              range: {
                min: "-30%",
                max: "100%",
              },
            },
          },
          //            title: '11',
          coordSystem: "polar",
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
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1", "Country code"] },
            y: {
              set: ["Joy factors", "Region"],
              range: {
                min: "-10%",
                max: "100%",
              },
            },
          },
          //            title: '12'
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1"] },
            y: {
              set: ["Joy factors", "Region", "Country code"],
              range: { min: "-10%" },
            },
          },
          //            title: '13'
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
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
            y: {
              set: null,
              range: { min: "-200%" },
            },
            lightness: { set: null },
          },
          //            title: '14'
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1", "Joy factors", "Region", "Country code"] },
            y: {
              set: null,
              range: {
                min: "0%",
                max: "100%",
              },
            },
          },
          //            title: '15'
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1", "Joy factors", "Region", "Country code"] },
            y: {
              set: ["Value 1"],
              range: { min: "-80%" },
            },
          },
          //            title: '16'
        },
        style: {
          plot: {
            marker: {
              borderWidth: 0,
              borderOpacity: 1,
            },
            yAxis: {
              label: { color: "#ffffff00" },
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Region"] },
            y: {
              set: ["Value 1", "Country code"],
              range: { min: "-20%" },
            },
          },
          //            title: '17'
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
              ticks: { color: null },
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors"] },
            y: {
              set: ["Value 1", "Region", "Country code"],
              range: { min: "0%" },
            },
          },
          //            title: '18'
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 0,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1", "Joy factors", "Region", "Country code"] },
            y: {
              set: null,
              range: { min: "-200%" },
            },
          },
          //            title: '19'
        },
      },
      {
        duration: step++ < targetstep ? "100ms" : "1400ms",
        x: { delay: 0 },
        y: { delay: 0 },
      }
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1", "Joy factors", "Region", "Country code"] },
            y: {
              set: null,
              range: { min: "0%" },
            },
          },
          //            title: '20',
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
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Value 1", "Joy factors", "Country code"] },
            y: { set: null },
          },
          //            title: '21'
        },
      },
      step++ < targetstep ? "100ms" : "100ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"] },
            y: {
              set: ["Value 1"],
              range: { max: "110%" },
            },
          },
          //            title: '22'
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: 1,
            },
          },
        },
      },
      {
        duration: step++ < targetstep ? "100ms" : "1400ms",
        style: { delay: "1.5s" },
      }
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"] },
            y: { set: ["Value 1", "Age group"] },
            color: { set: ["Age group"] },
          },
          //            title: '23'
        },
        style: {
          plot: {
            marker: {
              colorPalette: null,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"] },
            y: {
              set: ["Value 1", "Age group"],
              range: { max: "100%" },
            },
          },
          //            title: '24',
          split: true,
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"] },
            y: { set: ["Value 1", "Age group"] },
          },
          //            title: '25',
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
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"] },
            y: { set: ["Value 1", "Age group"] },
          },
          //            title: '26',
          align: "center",
          split: true,
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors", "Country code"] },
            y: { set: ["Value 1"] },
            noop: { set: ["Age group"] },
            color: { set: ["Joy factors"] },
          },
          //            title: '27',
          geometry: "circle",
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors"], range: { min: "0%" } },
            y: { set: ["Value 1"] },
            color: { set: ["Joy factors"] },
            lightness: { set: ["Value 3"] },
            noop: { set: ["Country code", "Age group"] },
            size: { set: ["Value 2"] },
          },
          //            title: '28',
          align: "min",
          orientation: "horizontal",
          split: false,
        },
        style: {
          plot: {
            marker: {
              rectangleSpacing: null,
              circleMaxRadius: 0.025,
              colorPalette: "#ee1b22FF #cecf07FF #01abf6FF #0ca749FF",
              fillOpacity: 0.1,
              borderOpacity: 0.1,
              borderWidth: 0,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { set: ["Joy factors"] },
            y: { set: ["Value 1"] },
            color: { set: ["Joy factors"] },
            lightness: { set: ["Value 3"] },
            noop: { set: ["Country code", "Age group"] },
            size: { set: ["Value 2"] },
          },
          //            title: '29',
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
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
          //            title: '30',
          align: "none",
        },
        style: {
          plot: {
            marker: {
              fillOpacity: null,
              borderOpacity: null,
              borderWidth: null,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
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
          //            title: '31',
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
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
          //            title:'32',
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          channels: {
            x: { attach: ["Hor"], range: { max: "688" } },
            y: { attach: ["Ver"], range: { max: "375" } },
            color: { set: ["Joy factors"] },
            label: { set: null },
          },
          title: null,
        },
        style: {
          plot: {
            marker: {
              circleMinRadius: 0.076,
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
      step++ < targetstep ? "100ms" : "1400ms"
    ),
  (chart) =>
    chart.animate(
      {
        config: {
          //          title: 'Appears the logo'
        },
        style: {
          logo: {
            filter: "opacity(1)",
          },
          plot: {
            marker: {
              fillOpacity: 0,
            },
          },
        },
      },
      step++ < targetstep ? "100ms" : "1400ms"
    ),
];

export default testSteps;
