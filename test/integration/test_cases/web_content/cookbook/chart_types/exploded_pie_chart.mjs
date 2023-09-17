import { data } from "../../../../test_data/chart_types_eu.mjs";

const testSteps = [
  (chart) => {
    return chart.animate({
      data: data,
      config: {
        channels: {
          x: ["Joy factors", "Value 2 (+)"],
          color: "Joy factors",
          label: "Value 2 (+)",
        },
        title: "Pie Chart",
        coordSystem: "polar",
      },
    });
  },
  (chart) => {
    class Shifter {
      constructor() {
        this.shifts = [];
      }

      permanent(context, dx, dy) {
        context.translate(dx, dy);
        this.shifts.push([dx, dy]);
      }

      temporal(context) {
        let shift = this.shifts.shift();
        if (!shift) shift = [0, 0];
        context.translate(...shift);
      }
    }

    let d = 0;
    let dMax = 10;
    let shifter;

    chart.on("update", (event) => {
      let p = event.detail.progress;
      d = Math.pow(p, 0.4) * dMax; // easing
      shifter = new Shifter();
    });

    chart.on("plot-marker-label-draw", (event) => {
      let ctx = event.renderingContext;
      shifter.temporal(ctx);
    });

    chart.on("plot-marker-draw", (event) => {
      let ctx = event.renderingContext;
      let rect = event.detail.rect;
      let angle = (rect.pos.x + rect.size.x / 2) * 2 * 3.14;
      let dAngle = rect.size.x * 2 * 3.14;
      let offset = d / 2 / Math.sin(dAngle / 2);
      let dx = +offset * Math.sin(angle);
      let dy = -offset * Math.cos(angle);
      shifter.permanent(ctx, dx, dy);
    });

    return chart.animate({
      config: {
        channels: {
          x: ["Joy factors", "Value 2 (+)"],
          color: "Joy factors",
          label: "Value 2 (+)",
        },
        title: "Exploded Pie Chart",
      },
      style: {
        plot: { overflow: "visible" },
      },
    });
  },
];

export default testSteps;
