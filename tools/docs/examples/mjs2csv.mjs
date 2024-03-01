class Js2csv {
	constructor(data) {
		this.data = data
	}

	addApostrophesIfContainsComma(value) {
		if (typeof value === 'string' && value.includes(',')) {
			return `"${value}"`
		}
		return value
	}

	getHeaderLine() {
		const header = []
		for (const series in this.data.series) {
			const value = this.addApostrophesIfContainsComma(this.data.series[series].name)
			header.push(value)
		}
		return header.join(',') + '\n'
	}

	getDataLine(i) {
		const line = []
		for (const key in this.data.series) {
			const value = this.addApostrophesIfContainsComma(this.data.series[key].values[i])
			line.push(value)
		}
		return line.join(',') + '\n'
	}

	getRecordLine(i) {
		const line = []
		for (const j in this.data.records[i]) {
			const value = this.addApostrophesIfContainsComma(this.data.records[i][j])
			line.push(value)
		}
		return line.join(',') + '\n'
	}

	convert() {
		let csv = ''
		csv += this.getHeaderLine()
		if (this.data.series[0].values) {
			for (let i = 0; i < this.data.series[0].values.length; i++) {
				csv += this.getDataLine(i)
			}
		} else {
			for (let i = 0; i < this.data.records.length; i++) {
				csv += this.getRecordLine(i)
			}
		}
		return csv
	}
}

const inputFilename = process.argv[2]

import(inputFilename).then((module) => {
	const js2csv = new Js2csv(module.data)
	console.log(js2csv.convert())
})
