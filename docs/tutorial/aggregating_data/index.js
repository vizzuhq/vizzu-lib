const dataLoaded = import('../../assets/data/music_data.js')
const mdChartLoaded = import('../../assets/javascripts/mdchart.js')

Promise.all([dataLoaded, mdChartLoaded]).then((results) => {
	const data = results[0].default
	const MdChart = results[1].default
	const mdchart = new MdChart(data, 'tutorial')
	mdchart.create([
		['01_a', '01_b', '01_c', '01_d'],
		['02_a', '02_b'],
		['03_a', '03_b'],
		['04_a', '04_b'],
		['05_a', '05_b'],
		['06_a', '06_b'],
		['07_a', '07_b']
	])
})
