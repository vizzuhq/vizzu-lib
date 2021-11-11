export default class Tooltip {
  constructor(chart) {
    this.set = false;
    this.chart = chart;
    this.id = 0;
    this.animating = false;
    this.lastMarkerId = null;
    this.lastMove = new Date();
    this.mouseMoveHandler = (event) => {
      this.mousemove(event);
    };
    this.mouseOnHandler = (event) => {
      this.mouseon(event);
    };
    this.enabled = false;
  }

  enable(enabled) {
    if (enabled === this.set) {
      return;
    }

    if (enabled) {
      this.chart.on("mousemove", this.mouseMoveHandler);
      this.chart.on("mouseon", this.mouseOnHandler);
    } else {
      this.chart.off("mousemove", this.mouseMoveHandler);
      this.chart.off("mouseon", this.mouseOnHandler);
      this.id++;
      setTimeout(() => {
        this.out(this.id);
      }, 200);
    }

    this.set = enabled;
  }

  mousemove() {
    this.lastMove = new Date();
  }

  mouseon(param) {
    this.id++;
    let id = this.id;
    if (!param.data.marker) {
      setTimeout(() => {
        this.out(id);
      }, 200);
    } else {
      setTimeout(() => {
        this.in(id, param.data.marker.id);
      }, 0);
    }
  }

  in(id, markerId) {
    if (this.id === id) {
      if (!this.animating) {
        this.lastMarkerId = markerId;
        //todo: check if animation is running
        this.animating = true;
        this.chart
          .animate(
            { config: { tooltip: markerId } },
            this.lastMarkerId ? "100ms" : "250ms"
          )
          .then(() => {
            this.animating = false;
          });
      } else {
        setTimeout(() => {
          this.in(id, markerId);
        }, 100);
      }
    }
  }

  out(id) {
    if (this.id === id) {
      let ellapsed = new Date() - this.lastMove;
      if (!this.animating && ellapsed > 200) {
        this.lastMarkerId = null;
        this.animating = true;
        this.chart.animate({ config: { tooltip: null } }, "250ms").then(() => {
          this.animating = false;
        });
      } else {
        setTimeout(() => {
          this.out(id);
        }, 200 - ellapsed);
      }
    }
  }
}
