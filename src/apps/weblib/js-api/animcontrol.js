export default class AnimControl extends Promise {
  constructor(executor, chart) {
    super((resolve, reject) => {
      executor(resolve, reject);
    });
    this.chart = chart;
  }

  then(onFulfilled, onRejected) {
    const nextp = super.then(onFulfilled, onRejected);
    nextp.chart = this.chart;
    return nextp;
  }

  get [Symbol.toStringTag]() {
    return "AnimControl";
  }

  seek(value) {
    this.animControl("seek", value);
    return this;
  }

  pause() {
    this.animControl("pause");
    return this;
  }

  play() {
    this.animControl("play");
    return this;
  }

  stop() {
    this.animControl("stop");
    return this;
  }

  reverse() {
    this.animControl("reverse");
    return this;
  }

  animControl(command, param = "") {
    let ccommand = this.chart.toCString(command);
    let cparam = this.chart.toCString(param);

    try {
      this.chart.call(this.chart.module._anim_control)(ccommand, cparam);
    } finally {
      this.chart.module._free(cparam);
      this.chart.module._free(ccommand);
    }
  }
}
