import Vizzu from './lib/vizzu.js';

let data = {
	series: [
		{
			name: 'Colors',
			type: 'categories',
			values: ['red', 'green', 'blue']
		},
		{
			name: 'Val',
			type: 'values',
			values: [ 3, 5, 4 ]
		}
	]
};

let chart = new Vizzu('vizzuCanvas');

let snapshot;

let anim = chart.initializing

.then(chart => chart.animate(
	{
		data: data,
		descriptor : {
			"channels.x.attach": [ 'Colors'],
			title: null,
			legend: null,
		},
		style: {
			"plot.marker.label.fontSize" : 20
		}
	}
))
.then(chart => {
	console.log(chart.styles.plot);
	return chart;
})
.then(chart => chart.animate(
	{
		data: {
			filter: record => record.Colors != 'blue'
		},
		descriptor : {
			channels: {
				x: { detach: [ 'Colors'] },
				y: { attach: [ 'Colors' ]}
			},
		}
	}
))
.then(chart => {
	snapshot = chart.store();
	return chart;
})
.then(chart => chart.animate(
	{
		descriptor : {
			channels: {
				color: { attach: [ 'Colors' ]}
			}
		}
	}
))
.then(chart => chart.animate(
	{
		data: {
			filter: null
		},
		descriptor : {
			channels: {
				color: { detach: [ 'Colors' ]},
				lightness: { attach: [ 'Colors' ]}
			}
		}
	}
))
.then(chart => chart.animate(
	{
		descriptor : {
			channels: {
				lightness: { detach: [ 'Colors' ]},
				label: { attach: [ 'Colors' ]}
			}
		}
	}
))
.then(chart => chart.animate(snapshot))
.catch(console.log);

let slider = document.getElementById("myRange");

slider.oninput = (e)=>
{
	let t = e.target.value;
	chart.animation.pause();
	chart.animation.seek(t/10 + '%');
};
