import Vizzu from 'https://cdn.jsdelivr.net/npm/vizzu@0.4.3/dist/vizzu.min.js';
//import Vizzu from '/example/lib/vizzu.js';
import getBase from './base.js';
import data from './data.js'

import DocId from '../documentid.js';

export default class VizzuView
{
	constructor(canvas)
	{
		this.canvasElement = document.getElementById(canvas);
		this.data = data;
		this.chart = new Vizzu(this.canvasElement);
		this.steps = [];
		this.stepMap = new Map();
		this.init();
		this.stack = [];
	}

	init()
	{
		this.anim = this.chart.initializing.then(chart => chart.animate({
			data: this.data
		})).then(chart => {
			this.stack.push({ 
				id: '0.0.0', 
				snapshot: chart.store(),
				title: ''
			});
			return chart;
		});
	}

	register(id, snippet)
	{
		this.steps.push(snippet);
		let index = this.steps.length - 1;
		this.stepMap.set(id, index);

		let baseId = (new DocId(id)).firstInSection();
		if (!this.stepMap.has(baseId))
			this.register(baseId, {
				id: baseId,
				fn: getBase((new DocId(baseId).section)),
				options: { title: '' }
			});
	}

	step(id)
	{
		let prevOfId = (new DocId(id)).prev().getSubSectionId();
		return this.anim.then(chart => {
			return this.goto(prevOfId).then(() => this.stepNext(id));
		});
	}

	stepNext(id)
	{
		return this.stepTo(id, true);
	}

	goto(id)
	{
		let last = this.stack[this.stack.length-1].id;

		if (id === last) return this.anim;

		let index = this.stepMap.get(id);
		if (index === undefined) return this.anim;

		let nextId = new DocId(id);
		let lastId = new DocId(last);

		if (!nextId.sameSection(lastId))
		{
			if(lastId.subsection > 0)
			{
				return this.stepBack(false).then(chart => {
					return this.goto(id); 
				});
			}
			else
			{
				let baseId = nextId.firstInSection();
				return this.stepTo(baseId, false).then(chart=> {
					return this.goto(id); 
				});
			}
		}
		else
		{
			if (nextId.subsection === lastId.subsection)
			{
				return this.anim
			}
			if (nextId.subsection > lastId.subsection)
			{
				let intermediateId = lastId.next().getSubSectionId();
				return this.stepTo(intermediateId, false)
				.then(chart => { 
					return this.goto(id)
				});
			}
			else
			{
				return this.stepBack(true).then(chart => { return this.goto(id); });
			}
		}
		return this.anim;
	}

	stepTo(id, normalPlay) 
	{
		let titleSpeed = normalPlay ? "300ms" : null;
		let animSpeed = normalPlay ? undefined : '250ms';

		let index = this.stepMap.get(id);
		if (index === undefined) return this.anim;

		let code = this.steps[index];

		this.anim.then(chart => {
			if (normalPlay)
				this.canvasElement.classList.remove('vizzu-canvas-rewind');
			else
				this.canvasElement.classList.add('vizzu-canvas-rewind');

			return chart;
		})

		return this.stepToCode(id, code, titleSpeed, animSpeed);
	}

	stepBack(animate)
	{
		let animSpeed = '250ms';

		return this.anim.then(chart => 
		{
			this.canvasElement.classList.add('vizzu-canvas-rewind');

			this.stack.pop();
			let lastState = this.stack[this.stack.length-1];

			if (animate)
				return this.anim.then(chart => {
					return chart.animate(lastState.snapshot, animSpeed)
				});
			else return chart;
		});
	}

	stepToCode(id, code, titleSpeed, animSpeed)
	{
		let title = code.options !== undefined 
			&& code.options.title !== undefined
			? code.options.title : '';

		return this.animTitle(title, titleSpeed).then(chart => 
		{
			if (animSpeed) chart.setAnimation(animSpeed);
			code.fn(chart); 
			return this.chart.anim;
		})
		.then(chart => {
			this.stack.push({ id, snapshot: chart.store(), title });
			return chart;
		});
	}

	animTitle(title, titleSpeed)
	{
		return this.anim.then(chart =>
		{
			if (titleSpeed !== null)
				return chart.animate({ config: { title }}, titleSpeed);
			else return chart;
		});
	}
}