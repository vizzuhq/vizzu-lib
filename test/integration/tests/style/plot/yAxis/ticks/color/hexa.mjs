import Chart from "../../../../../chart.mjs";

const testCases = [
  {
    testName: "static_blueCustom_0.25",
    testSteps: [
      Chart.animate({
        data: "simpleDataWithThreeValue",
        config: "simpleDataCircleGeometryVerticalOrientation",
        style: {
          plot: {
            yAxis: {
              ticks: {
                length: 5,
                lineWidth: 5,
                color: "#1A2B3C40",
              },
            },
          },
        },
      }),
    ],
  },
  {
    testName: "static_blueCustom_0.75",
    testSteps: [
      Chart.animate({
        data: "simpleDataWithThreeValue",
        config: "simpleDataCircleGeometryVerticalOrientation",
        style: {
          plot: {
            yAxis: {
              ticks: {
                length: 5,
                lineWidth: 5,
                color: "#1A2B3CBF",
              },
            },
          },
        },
      }),
    ],
  },
  {
    testName: "static_yellowCustom_0.25",
    testSteps: [
      Chart.animate({
        data: "simpleDataWithThreeValue",
        config: "simpleDataCircleGeometryVerticalOrientation",
        style: {
          plot: {
            yAxis: {
              ticks: {
                length: 5,
                lineWidth: 5,
                color: "#E5D4C340",
              },
            },
          },
        },
      }),
    ],
  },
  {
    testName: "static_yellowCustom_0.75",
    testSteps: [
      Chart.animate({
        data: "simpleDataWithThreeValue",
        config: "simpleDataCircleGeometryVerticalOrientation",
        style: {
          plot: {
            yAxis: {
              ticks: {
                length: 5,
                lineWidth: 5,
                color: "#E5D4C3BF",
              },
            },
          },
        },
      }),
    ],
  },
  {
    testName: "animated_yellowCustom_0.25-blueCustom_0.75",
    testSteps: [
      Chart.animate({
        data: "simpleDataWithThreeValue",
        config: "simpleDataCircleGeometryVerticalOrientation",
        style: {
          plot: {
            yAxis: {
              ticks: {
                length: 5,
                lineWidth: 5,
                color: "#E5D4C340",
              },
            },
          },
        },
      }),
      Chart.animate({
        style: {
          plot: {
            yAxis: {
              ticks: {
                color: "#1A2B3CBF",
              },
            },
          },
        },
      }),
    ],
  },
  {
    testName: "animated_yellowCustom_0.75-blueCustom_0.25",
    testSteps: [
      Chart.animate({
        data: "simpleDataWithThreeValue",
        config: "simpleDataCircleGeometryVerticalOrientation",
        style: {
          plot: {
            yAxis: {
              ticks: {
                length: 5,
                lineWidth: 5,
                color: "#E5D4C3BF",
              },
            },
          },
        },
      }),
      Chart.animate({
        style: {
          plot: {
            yAxis: {
              ticks: {
                color: "#1A2B3C40",
              },
            },
          },
        },
      }),
    ],
  },
];

export default testCases;
