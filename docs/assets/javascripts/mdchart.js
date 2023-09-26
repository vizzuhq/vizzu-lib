class MdChart {
  constructor(data, vizzu, id) {
    this.data = data;
    this.vizzuLoaded = import(vizzu).then((vizzuUrl) => {
      return import(vizzuUrl.default);
    });
    this.id = id;
  }

  create(snippets) {
    let chart = Promise.resolve();
    for (let i = 0; i < snippets.length; i++) {
      const number = i + 1;
      chart = this.animate(("0" + number).slice(-2), snippets[i], chart);
    }
  }

  restore(number, snippet, prevChart, snapshot, chart) {
    const div = document.getElementById(this.id + "_" + number);

    return Promise.all([chart, prevChart]).then((results) => {
      let chart = results[0];
      const prevChart = results[1];
      div.classList.remove("loading");
      div.classList.add("playing");
      let animTarget;
      if (snapshot && !prevChart) {
        animTarget = snapshot;
      } else {
        animTarget = {};
        if (prevChart) {
          animTarget.config = Object.assign({}, prevChart.config);
          animTarget.style = Object.assign({}, prevChart.style);
          // remove if it can be found in the prevChart
          if (snippet.initDataFilter) {
            animTarget.data = { filter: snippet.initDataFilter };
          }
        }
      }
      chart = chart.animate(animTarget);
      chart.activated.then((control) => control.seek("100%"));
      return chart;
    });
  }

  animate(number, snippet, prevChart) {
    const div = document.getElementById(this.id + "_" + number);
    div.classList.add("loading");

    let snapshot;

    let chart = this.vizzuLoaded.then((Vizzu) => {
      const VizzuConstructor = Vizzu.default;
      return new VizzuConstructor(div, { data: this.data }).initializing;
    });

    chart = this.restore(number, snippet, prevChart, snapshot, chart);

    chart = chart.then((chart) => {
      snapshot = chart.store();
      return chart;
    });

    let firstRun = true;
    let clicked = false;
    div.onclick = () => {
      if (!clicked) {
        clicked = true;

        chart = this.restore(number, snippet, prevChart, snapshot, chart);
        chart.then(() => {
          div.classList.remove("replay");
          div.classList.add("playing");
        });
        for (let i = 0; i < snippet.anims.length; i++) {
          chart = chart.then((chart) => {
            chart = snippet.anims[i](chart, {});
            if (this.id === "tutorial" && firstRun && chart.activated) {
              chart.activated.then((control) => control.seek("100%"));
            }
            return chart;
          });
        }
        chart.then(() => {
          div.classList.remove("playing");
          div.classList.add("replay");
          clicked = false;
          firstRun = false;
        });

        return chart;
      }
    };
    div.click();

    return chart;
  }
}

export default MdChart;
