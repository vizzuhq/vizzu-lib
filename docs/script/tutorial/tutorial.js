import VizzuView from "./vizzu-view.js";
import DomHelper from "../dom-helper.js";
import DocId from "../documentid.js";

export default class tutorial
{
	constructor(snippetRegistry)
	{
		this.subtitles = new Map();
		this.menus = new Map();

		this.page = document.getElementById('page-0');
		this.page.onscroll = event => this.scrolled(event);

		this.discover();

		this.setupVizzu(snippetRegistry);

		window.onresize = () => {
			this.setVizzuPosition(this.activeSnippet, false);
		}
	}

	scrolled(event)
	{
		const topSectionId = this.firstVisibleSubtitle();
		this.sectionScrolledIn(topSectionId);
	}

	navigateToId(id)
	{
		this.setInitialSnippet(id);

		let sectionId = (new DocId(id)).getSectionId();

		this.subtitles.get(sectionId)
			.scrollIntoView({ behavior: 'auto' });
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
		else {
			this.activeSnippet = element;
			element.focus({ preventScroll: true });
		}
	}

	setupVizzu(snippetRegistry)
	{
		this.vizzuView = new VizzuView('vizzu-canvas');

		for (let [id, snippet] of snippetRegistry.snippets)
			this.vizzuView.register(id, snippet);
	}

	discover()
	{
		let subtitles = document.getElementsByClassName('subtitle');
		for (let subtitle of subtitles) {
			const id = DomHelper.parseId(subtitle).id;
			this.subtitles.set(id, subtitle);
		}

		let submenus = document.getElementsByClassName('submenuitem');
		for (let submenu of submenus)
		{
			const id = DomHelper.parseId(submenu).id;
			if ((new DocId(id)).document == 0)
			{
				submenu.onclick = () => {
					history.pushState({ id }, '', `#chapter-${id}`)
					this.onMenu(id);
					this.subtitles.get(id).scrollIntoView({ behavior: 'smooth' });
					this.setInitialSnippet(id+'.1');
				};
		
				this.menus.set(id, submenu);
			}
		}

		let snippets = document.getElementsByClassName('runable');
		for (let snippet of snippets)
		{
			snippet.onclick = () => { this.activateSnippet(snippet); };
			snippet.onfocus = () => { this.activateSnippet(snippet); };
			snippet.onkeydown = (e) => {
				if (e.which === 13) this.activateSnippet(snippet);
			};
		}
	}

	activateSnippet(snippet)
	{
		this.activeSnippet = snippet;
		this.setVizzuPosition(snippet, true);
		const id = DomHelper.parseId(snippet).id;
		this.vizzuView.step(id);
	}

	setVizzuPosition(snippet, transition)
	{
		if (this.activeSnippet == undefined) return;

		let canvas = document.getElementById('vizzu-canvas');

		let targetTop = snippet.offsetTop 
			+ snippet.getBoundingClientRect().height / 2
			- canvas.getBoundingClientRect().height / 2;

		let view = document.getElementById('vizzu-view');
		
		let left = parseInt(window.getComputedStyle(view).left, 10);
		view.style.top = (left > 0 ? targetTop : 0)+ 'px';
		
		view.style.transition = `top ${transition ? .2 : 0}s`;
	}

	firstVisibleSubtitle()
	{
		let last = '0.0';
		for (let [ id, subtitle ] of this.subtitles)
		{
			if (this.isAboveViewCenter(subtitle))
				return last;
			last = id;
		}
		return last;
	}

	isAboveViewCenter(child)
	{
		const childRect = child.getBoundingClientRect();
		const parentRect = this.page.getBoundingClientRect();
		return childRect.top > parentRect.top + parentRect.height/2;
	}
}
