import Vizzu from '../../example/lib/vizzu.js';
import DocId from './documentid.js';

export default class VizzuView
{
	constructor(canvas, data)
	{
		this.canvasElement = canvas;
		this.data = data;
		this.chart = new Vizzu(this.canvasElement);
		this.steps = [];
		this.stepMap = new Map();
		this.init();
		this.stack = [];
	}

	init()
	{
		this.anim = this.chart.initializing.then(chart => {
			chart.module._vizzu_setLogging(true);
			chart.on('logo-draw', event => { event.preventDefault() });
			return chart;
		}).then(chart => chart.animate({
			data: this.data
		})).then(chart => {
			this.stack.push({ id: '0.0.0', snapshot: chart.store() });
			return chart;
		});;
	}

	register(id, code)
	{
		this.steps.push(code);
		let index = this.steps.length - 1;
		this.stepMap.set(id, index)
	}

	step(id, titleSpeed = '300ms')
	{
		let index = this.stepMap.get(id);
		if (index === undefined) return;

		let nextId = new DocId(id);
		let lastId = new DocId(this.stack.at(-1).id);

		if (!nextId.sameSection(lastId))
		{
			if(nextId.subsection > 0)
			{
				let lastState = this.stack.pop();
				
				this.anim = this.anim
					.then(chart => chart.animate(lastState, '250ms'))
					.then(chart => {
						this.step(id, titleSpeed);
						return chart;
					});
			}
			else
			{
				this.anim = this.anim
					.then(chart => {
						let baseId = nextId.firstInSection();
						this.step(baseId, 0).then(chart => {
							this.step(id, titleSpeed);
							return chart;
						});

						return chart;
					})
			}
		}
		else
		{
			let code = this.steps[index];

			this.anim = this.anim.then(
				chart => chart.animate({ config: { title: code.title }}, titleSpeed));
			
			this.anim = this.anim.then(code.func).then(chart => {
				this.stack.push({ id: id, snapshot: chart.store() });
				return chart;
			});
		}

		return this.anim;
	}
}