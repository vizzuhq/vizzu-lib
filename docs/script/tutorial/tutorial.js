import VizzuView from "./vizzu-view.js";
import DomHelper from "../dom-helper.js";
import DocId from "../documentid.js";
import Section from "./section.js";

export default class tutorial
{
	constructor(snippetRegistry)
	{
		this.lastSection = null;
		this.sections = new Map();

		this.page = document.getElementById('page-0');
		this.page.onscroll = event => this.scrolled(event);

		this.discover();

		this.setupVizzu(snippetRegistry);
	}

	scrolled(event)
	{
		const topSectionId = this.firstVisibleSubtitle();
		if (topSectionId != this.lastSection)
			this.activateSection(topSectionId)
	}

	gotoInitSection()
	{
		let hashId = window.location.hash;
		if (hashId !== '')
		{
			let sectionId = DomHelper.parseIdString(hashId).id;

			this.setInitialSnippet(sectionId);
	
			this.getSection(sectionId).element
				.scrollIntoView({ behavior: 'auto' });
		}
		else
		{
			this.setInitialSnippet();
		}
	}

	activateSection(id)
	{
		if (this.lastSection !== null)
			this.getSection(this.lastSection).select(false);

		this.getSection(id).select(true);

		this.lastSection = id;
	}

	getSection(id)
	{
		let sectionId = (new DocId(id)).getSectionId();
		return this.sections.get(sectionId);
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
		this.vizzuView = new VizzuView('vizzu-canvas');

		for (let [id, snippet] of snippetRegistry.snippets)
			this.vizzuView.register(id, snippet);
	}

	discover()
	{
		let subtitles = document.getElementsByClassName('subtitle');
		for (let subtitle of subtitles) {
			const id = DomHelper.parseId(subtitle).id;
			this.sections.set(id, new Section(subtitle));
		}

		let submenus = document.getElementsByClassName('submenuitem');
		for (let submenu of submenus)
		{
			const id = DomHelper.parseId(submenu).id;
			if ((new DocId(id)).document == 0)
			{
				let section = this.sections.get(id);
				if (section) section.setMenu(submenu);	
			}
		}

		let snippets = document.getElementsByClassName('runable');
		for (let snippet of snippets)
		{
			snippet.onclick = () => { this.activateSnippet(snippet); };
			snippet.onfocus = () => { this.activateSnippet(snippet); };
		}
	}

	activateSnippet(snippet)
	{
		let canvas = document.getElementById('vizzu-canvas');

		let targetTop = snippet.offsetTop 
			+ snippet.getBoundingClientRect().height / 2
			- canvas.getBoundingClientRect().height / 2;

		let view = document.getElementById('vizzu-view');
		
		view.style.top = targetTop + 'px';
		view.style.transition = 'top .2s';

		const id = DomHelper.parseId(snippet).id;
		this.vizzuView.step(id);
	}

	firstVisibleSubtitle()
	{
		let last = '0.0';
		for (let [ id, section ] of this.sections)
		{
			if (this.isAboveViewCenter(section.element))
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
