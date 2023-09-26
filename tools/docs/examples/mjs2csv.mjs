class Js2csv {
  constructor(data) {
    this.data = data;
  }

  getHeaderLine() {
    const header = [];
    for (const series in this.data.series) {
      header.push(this.data.series[series].name);
    }
    return header.join(",") + "\n";
  }

  getDataLine(i) {
    const line = [];
    for (const key in this.data.series) {
      line.push(this.data.series[key].values[i]);
    }
    return line.join(",") + "\n";
  }

  getRecordLine(i) {
    const line = this.data.records[i];
    return line.join(",") + "\n";
  }

  convert() {
    let csv = "";
    csv += this.getHeaderLine();
    if (this.data.series[0].values) {
      for (let i = 0; i < this.data.series[0].values.length; i++) {
        csv += this.getDataLine(i);
      }
    } else {
      for (let i = 0; i < this.data.records.length; i++) {
        csv += this.getRecordLine(i);
      }
    }
    return csv;
  }
}

const inputFilename = process.argv[2];

import(inputFilename).then((module) => {
  const js2csv = new Js2csv(module.data);
  console.log(js2csv.convert());
});
