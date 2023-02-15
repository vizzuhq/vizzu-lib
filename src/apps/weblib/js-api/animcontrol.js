export class Animation {}

export class AnimControl {
  constructor(chart) {
    this.chart = chart;
  }

  get [Symbol.toStringTag]() {
    return "AnimControl";
  }

  store() {
    return this.chart._objectRegistry.get(
      this.chart._call(this.chart.module._chart_anim_store),
      Animation
    );
  }

  seek(value) {
    this._animControl("seek", value);
    return this;
  }

  pause() {
    this._animControl("pause");
    return this;
  }

  play() {
    this._animControl("play");
    return this;
  }

  stop() {
    this._animControl("stop");
    return this;
  }

  cancel() {
    this._animControl("cancel");
    return this;
  }

  reverse() {
    this._animControl("reverse");
    return this;
  }

  _animControl(command, param = "") {
    let ccommand = this.chart._toCString(command);
    let cparam = this.chart._toCString(param);

    try {
      this.chart._call(this.chart.module._anim_control)(ccommand, cparam);
    } finally {
      this.chart.module._free(cparam);
      this.chart.module._free(ccommand);
    }
  }
}
