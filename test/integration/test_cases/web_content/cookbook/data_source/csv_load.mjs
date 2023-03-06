const testSteps = [
	async chart => 
	{
		await import("https://d3js.org/d3.v7.min.js");

		let data = { series: [], records: [] };

		data.records = await d3.csv("https://raw.githubusercontent.com/vizzuhq/vizzu-lib-doc/story_galery/docs/datasets/population_total_long.csv");

		data.series = Object.keys(data.records[0]).map(name => ({ 
			name: name,
			type: name == 'Count' ? 'measure' : 'dimension'
		}));

		for (let i = 0; i < data.records.length; i++)
			data.records[i] = Object.values(data.records[i]);

		chart.animate({
			data,
			config: {
				x: 'Year',
				y: 'Count',
				geometry: 'line'
			},
			style: { "plot.xAxis.label": { 
				angle: '-45deg'
			} }
		})
	}
];

export default testSteps;