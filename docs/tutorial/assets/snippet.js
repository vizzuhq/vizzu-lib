const mdChartLoaded = import('../../assets/javascripts/mdchart.js')

const scriptTag = document.currentScript
const dataFile = scriptTag.getAttribute('data') ?? '../../assets/data/music_data.js'
const configFile = scriptTag.getAttribute('config')

const dataLoaded = import(dataFile)
const configLoaded = import(configFile)

Promise.all([mdChartLoaded, dataLoaded, configLoaded]).then((results) => {
	const data = results[1].default
	const config = results[2].default
	const MdChart = results[0].default
	const mdchart = new MdChart(data, 'tutorial')
	mdchart.createFromConfig(config)
})
