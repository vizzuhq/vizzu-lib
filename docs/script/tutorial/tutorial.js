import VizzuView from "./vizzu-view.js";
import DomHelper from "../dom-helper.js";

export default class tutorial
{
	constructor(snippetRegistry)
	{
		this.discover();

		this.setupVizzu(snippetRegistry);
	}

	setupVizzu(snippetRegistry)
	{
		this.vizzuView = new VizzuView('example-canvas');

		for (let [id, snippet] of snippetRegistry.snippets)
			this.vizzuView.register(id, snippet);

		document.getElementById('snippet-0.2.1').focus({ preventScroll: true });
	}

	discover()
	{
		let snippets = document.getElementsByClassName('runable');
		for (let snippet of snippets)
		{
			snippet.onclick = () => { this.activateSnippet(snippet); };
			snippet.onfocus = () => { this.activateSnippet(snippet); };
		}
	}

	activateSnippet(snippet)
	{
		let canvas = document.getElementById('example-canvas');

		let targetTop = snippet.offsetTop 
			+ snippet.getBoundingClientRect().height / 2
			- canvas.getBoundingClientRect().height / 2;

		let view = document.getElementById('example-view');
		
		view.style.top = targetTop + 'px';
		view.style.transition = 'top .2s';

		const id = DomHelper.parseId(snippet).id;
		this.vizzuView.step(id);
	}
}
