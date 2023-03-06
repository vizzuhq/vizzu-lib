import { data_6 } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
	chart => chart.animate({
		data: data_6,
		config: {
			x: 'Joy factors',
			y: 'Value 2 (+)',
			color: 'Joy factors',
			label: 'Value 2 (+)'
		}
	},0),

	chart => 
	{
		let lastMouseX = null;
		let progress = null;

		let animation = chart.animate({
			x: 'Country',
			y: 'Value 2 (+)',
			color: 'Country',
			label: 'Value 2 (+)'
		});

		let seek = (percent) =>
		{
			animation.activated.then(control => {
				control.pause();
				control.seek(percent + '%');
			})
		};

		chart.on('update', (ev) => { progress = ev.data.progress; })

		chart.on("click", (event) => { event.preventDefault(); });

		chart.on("mousedown", (event) => {
			lastMouseX = event.data.coords.x;
			event.preventDefault();
		});

		chart.on("mouseup", (event) => {
			lastMouseX = null;
			event.preventDefault();
		});

		chart.on("mousemove", (event) => {
			if (lastMouseX)
			{
				let mouseX = event.data.coords.x;
				let diff = 2 * (mouseX - lastMouseX);
				seek((progress + diff) * 100);
				lastMouseX = mouseX;
			}
			event.preventDefault();
		});

		return animation;
	}
];

export default testSteps;