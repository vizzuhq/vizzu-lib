import Vizzu from '../../example/lib/vizzu.js';

export default class VizzuView
{
	constructor(canvas, data)
	{
		this.canvasElement = canvas;
		this.data = data;
		this.chart = new Vizzu(this.canvasElement);
		this.init();
	}

	init()
	{
		this.anim = this.chart.initializing.then(chart => {
			chart.module._vizzu_setLogging(true);
			chart.on('logo-draw', event => { event.preventDefault() });
			return chart;
		}).then(chart => chart.animate({
			data: this.data
		}));
	}

	step(code)
	{
		this.anim = this.anim.then(
			chart => chart.animate({ config: { title: code.title }}, '300ms'));
		
		this.anim = this.anim.then(code.func);

		return this.anim;
	}
}