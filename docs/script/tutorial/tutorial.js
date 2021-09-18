import VizzuView from "./vizzu-view.js";
import DomHelper from "../dom-helper.js";
import DocId from "../documentid.js";

export default class tutorial
{
	constructor(snippetRegistry)
	{
		this.discover();

		this.setupVizzu(snippetRegistry);
	}

	setInitialSnippet(id)
	{
		const defaultId = '0.2.1';
		
		if (id === undefined) id = defaultId;
		let initId = new DocId(id);
		if (initId.document !== 0) id = defaultId;
		if (initId.subsection === undefined) initId.subsection = 1;
		
		let elementId = `snippet-${initId.getSubSectionId()}`;
		let element = document.getElementById(elementId);

		if (element === null) this.setInitialSnippet(defaultId);
		else element.focus({ preventScroll: true });
	}

	setupVizzu(snippetRegistry)
	{
		this.vizzuView = new VizzuView('example-canvas');

		for (let [id, snippet] of snippetRegistry.snippets)
			this.vizzuView.register(id, snippet);
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
