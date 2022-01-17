import Vizzu from './lib/vizzu.js';

let data = {
	series: [
		{
			name: 'Colors',
			type: 'dimension',
			values: ['red', 'green', 'blue',
			'red1', 'green1', 'blue1',
			'red2', 'green2', 'blue2',
			'red3', 'green3', 'blue3',
			'red4', 'green4', 'blue4',
		]
		},
		{
			name: 'Uni',
			type: 'dimension',
			values: ['bla']
		},
		{
			name: 'Val',
			type: 'measure',
			values: [ 3, 5, 4, 
				3+1, 5+1, 4+1,
				3+2, 5+2, 4+2 ,
				3+3, 5+3, 4+3 ,
				3+4, 5+4, 4+4 
			]
		},
		{
			name: 'Val2',
			type: 'measure',
			values: [ Math.random(), Math.random(), Math.random(), 
				Math.random(), Math.random(), Math.random(),
				Math.random(), Math.random(), Math.random() ,
				Math.random(), Math.random(), Math.random() ,
				Math.random(), Math.random(), Math.random() 
			]
		},
		{
			name: 'Val3',
			type: 'measure',
			values: [ Math.random(), Math.random(), Math.random(), 
				Math.random(), Math.random(), Math.random(),
				Math.random(), Math.random(), Math.random() ,
				Math.random(), Math.random(), Math.random() ,
				Math.random(), Math.random(), Math.random() 
			]
		}
	]
};

let chart = new Vizzu('vizzuCanvas');

let snapshot;

let anim = chart.initializing
.then(chart => {
	chart.feature('tooltip',true);
	chart.module._vizzu_setLogging(true);
	return chart;
})
.then(chart => chart.animate(
	{
		data: data,
		config: {
			channels: {
				y: 'Colors',
				x: 'Val',
				label: 'Val',
				size: 'Val3',
				color: 'Colors'
			},
			geometry: 'circle',
			title: null,
			legend: null,
		},
		style: {
			tooltip: {
				layout: "multiLine",
				shadowColor: 'rgba(0,0,0,0.1)'
			}
		}
	}
))
.then(chart => chart.animate({ 
//	config: { geometry: 'rectangle' },
	style: { plot: { marker: { colorPalette: '#0055e8FF #003456FF #00AF10FF' } } } 
}))
.then(chart => chart.animate({ 
	style: null 
}))
/*.then(chart => chart.animate({ tooltip: 1 }))
.then(chart => chart.animate({ tooltip: 14 }))
.then(chart => chart.animate({ tooltip: 10 }))
.then(chart => chart.animate({ tooltip: 3 }))
.then(chart => chart.animate({ tooltip: 13 }))
.then(chart => chart.animate({ tooltip: 8 }))*/
.catch(console.log);

console.log(chart);

let slider = document.getElementById("myRange");

slider.oninput = (e)=>
{
	let t = e.target.value;
	anim.pause().seek(t/10 + '%');
};
