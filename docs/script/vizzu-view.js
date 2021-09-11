import Vizzu from '../../example/lib/vizzu.js';
import data from './sample-data.js';
import style from './example-style.js';

export default class VizzuView
{
	constructor(canvasName)
	{
		this.chart = new Vizzu(document.getElementById(canvasName));
		this.init();
	}

	init()
	{
		this.anim = this.chart.initializing.then(chart =>
			chart.animate({
				data: data,
				style: style,
				config: {
					channels: {
						x: 'Timeseries',
						y: 'Values 1',
						size: 'Values 1'
					},
					title: null
				}
			})
		)
	}

	step(func)
	{
		this.anim = this.anim.then(func);
	}

}