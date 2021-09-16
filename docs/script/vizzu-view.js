import Vizzu from 'https://vizzu-lib-main.storage.googleapis.com/lib/vizzu.js';
import getBase from '../content/tutorial/base.js';
import data from '../content/tutorial/data.js'

import DocId from './documentid.js';

export default class VizzuView
{
	constructor(canvas)
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
				title: '', 
				fn: getBase((new DocId(baseId).section))
			});
	}

	step(id)
	{
		console.log('step '+id);
		let prevOfId = (new DocId(id)).prev().getSubSectionId();
		return this.goto(prevOfId).then(() => this.stepNext(id));
	}

	stepNext(id)
	{
		console.log('step next'+id);
		return this.stepTo(id, "300ms", undefined);
	}

	goto(id)
	{
		let last = this.stack.at(-1).id;
		console.log('goto '+id+' from '+last);

		if (id === last) return this.anim;

		let index = this.stepMap.get(id);
		if (index === undefined) return this.anim;

		let nextId = new DocId(id);
		let lastId = new DocId(last);

		if (!nextId.sameSection(lastId))
		{
			if(lastId.subsection > 0)
			{
				return this.stepBack().then(chart => {
					return this.goto(id); 
				});
			}
			else
			{
				let baseId = nextId.firstInSection();
				return this.stepTo(baseId, null, '250ms').then(chart=> {
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
				console.log(lastId+' -> '+intermediateId);
				return this.stepTo(intermediateId, null, '250ms')
				.then(chart => { 
					return this.goto(id)
				});
			}
			else
			{
				return this.stepBack().then(chart => { return this.goto(id); });
			}
		}
		return this.anim;
	}

	stepTo(id, titleSpeed, animSpeed) 
	{
		let index = this.stepMap.get(id);
		if (index === undefined) return this.anim;

		let code = this.steps[index];

		return this.stepToCode(id, code, titleSpeed, animSpeed);
	}

	stepBack(titleSpeed = null, animSpeed = '250ms')
	{
		return this.anim.then(chart => 
		{
			this.stack.pop();
			let lastState = this.stack.at(-1);
			return this.animTitle(lastState.title, titleSpeed)
				.then(chart => {
					console.log('step back' + lastState.id);
					return chart.animate(lastState.snapshot, animSpeed)
				});
		});
	}

	stepToCode(id, code, titleSpeed, animSpeed)
	{
		return this.animTitle(code.title, titleSpeed).then(chart => 
		{
			if (animSpeed) chart.setAnimation(animSpeed);
			console.log('step to code'+id);
			code.fn(chart); 
			return this.chart.anim;
		})
		.then(chart => {
			this.stack.push({ 
				id: id, 
				snapshot: chart.store(), 
				title: code.title
			});
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