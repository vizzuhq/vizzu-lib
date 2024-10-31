const mdChartLoaded = import('../../assets/javascripts/mdchart.js')

const scriptTag = document.currentScript
const dataFile = scriptTag.getAttribute('data') ?? '../../assets/data/music_data.js'

const dataLoaded = import(dataFile)
const configLoaded = fetch('./config.json').then((response) => response.json())

Promise.all([mdChartLoaded, dataLoaded, configLoaded]).then((results) => {
	const data = results[1].default
	const config = results[2]
	const MdChart = results[0].default
	const mdchart = new MdChart(data, 'tutorial')
	mdchart.create(config)
})
