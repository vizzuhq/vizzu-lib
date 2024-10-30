const mdChartLoaded = import('../assets/javascripts/mdchart.js')

const scriptTag = document.currentScript
const configFile = scriptTag.getAttribute('config')

const dataLoaded = import('../assets/data/music_data.js')
const configLoaded = fetch(configFile).then((response) => response.json())

Promise.all([mdChartLoaded, dataLoaded, configLoaded]).then((results) => {
	const data = results[1].default
	const config = results[2]
	const MdChart = results[0].default
	const mdchart = new MdChart(data, 'tutorial')
	mdchart.create(config)
})
