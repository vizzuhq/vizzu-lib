import Vizzu from '../../example/lib/vizzu.js';

export default class VizzuView
{
	constructor(canvasName)
	{
		this.chart = new Vizzu(document.getElementById(canvasName));
		this.init();
	}

	init()
	{
		this.anim = this.chart.initializing.then(chart => {
			chart.module._vizzu_setLogging(true);
			chart.on('logo-draw', event => { event.preventDefault() });
			return chart;
		}).then(chart => chart.animate({
			style: { 
				fontSize: '.90em',
				plot: { 
					paddingTop: 30,
					paddingBottom: 25
				},
				title: {
					fontSize: '1.5em',
					paddingBottom: 0 
				}
			}
		}));
	}

	step(title, func)
	{
		this.anim = this.anim.then(
			chart => chart.animate({ config: { title }}, '300ms'));
		
		this.anim = this.anim.then(func);
	}

}