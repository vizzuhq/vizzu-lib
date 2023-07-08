import Vizzu from 'https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js';

let animopts = {
	x: { delay: 0, duration: .7 },
	y: { delay: 0, duration: .7 },
	style: { delay: 0, duration: .7 },
}

let chart = new Vizzu("axis-demo", {
	data: {
		series: [
			{ name: 'Dimension', values: ['Alice', 'Bob', 'Ted'] },
			{ name: 'Measure', values: [4, 3, 2] }
		]
	},
	config: {
		legend: null,
		x: { step: 1, range: { max: '100%' } }
	}, style: {
		fontSize: "1em",
		plot: { marker: { rectangleSpacing: 0 } },
		paddingBottom: 0,
		paddingTop: 0,
		paddingLeft: 0,
		paddingRight: 0
	}
});

chart.initializing.then(chart => {
	chart.on('logo-draw', e => e.preventDefault());
	return chart;
})

let f0 = () => {
	chart.animate({
		config: {
			x: 'Dimension',
			label: 'Dimension',
			color: 'Dimension',
			orientation: 'vertical'
		},
		style: {
			"plot.marker.label": {
				filter: 'lightness(0.7)',
				paddingLeft: null
			}
		}
	}, animopts)
};


let f1 = () => {
	chart.animate({
		config: {
			x: 'Measure',
			color: 'Dimension',
			label: 'Dimension',
			orientation: 'vertical'
		},
		style: {
			"plot.marker.label": {
				filter: 'lightness(0.7)',
				paddingLeft: '-2.4em'
			}
		}
	}, animopts);
}

let f2 = () => {
	chart.animate({
		config: {
			x: ['Measure', 'Dimension'],
			color: 'Dimension',
			label: 'Dimension',
			orientation: 'vertical'
		},
		style: {
			"plot.marker.label": {
				filter: 'lightness(0.7)',
				paddingLeft: null
			}
		}
	}, animopts);
}

f0();

document.getElementById('axis-demo-0').onclick = f0;
document.getElementById('axis-demo-1').onclick = f1;
document.getElementById('axis-demo-2').onclick = f2;
