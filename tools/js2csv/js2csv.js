const fs = require('fs');

class Js2csv {
	constructor(data) {
		this.data = data;
	}

	getHeaderLine() {
		let header = [];
		for (let series in this.data.series) {
			header.push(this.data.series[series].name);
		}
		return header.join(',') + '\n';
	}

	getDataLine(i) {
		let line = [];
		for (let key in this.data.series) {
			line.push(this.data.series[key].values[i]);
		}
		return line.join(',') + '\n';
	}

	convert() {
		let csv = '';
		csv += this.getHeaderLine();
		for (let i = 0; i < this.data.series[0].values.length; i++) {
			csv += this.getDataLine(i);
		}
		return csv;
	}

	write(fileName) {
		fs.writeFile(fileName, this.convert(), (err) => {
			if (err) throw err;
			console.log('The file has been saved!');
		});
	}
}

modules.export = Js2csv;

