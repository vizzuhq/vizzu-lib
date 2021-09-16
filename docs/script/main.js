import DomHelper from "./dom-helper.js";
import Section from "./section.js";
import Snippet from "./snippet.js";
import DocId from "./documentid.js";
import VizzuView from './vizzu-view.js';

export default class Main
{
	constructor(snippetRegistry)
	{
		this.sections = new Map();
		this.snippets = new Map();

		this.discover();

		this.setupVizzu(snippetRegistry);

		this.lastId = null;

		this.contentView = document.getElementById('content-view');
	}

	setupVizzu(snippetRegistry)
	{
		this.vizzuView = new VizzuView('example-canvas');

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
			let section = this.sections.get(id);
			if (section) section.setMenu(submenu);
		}

		let snippets = document.getElementsByClassName('snippet');
		for (let snippet of snippets)
		{
			const id = DomHelper.parseId(snippet).id;
			snippet.onclick = () => { this.activate(id); };
			this.snippets.set(id, new Snippet(snippet));
		}
	}
/*
	scrolled(event)
	{
		const topSectionId = this.firstVisibleSectionId();
		if (topSectionId != this.navigation.lastId) {
			this.navigation.nextId = topSectionId;
			this.navigation.currentMove++;
			let move = this.navigation.currentMove;
			setTimeout(() => {
				if (this.navigation.currentMove === move) this.activate()
			}, 300);
		}
	}
*/
	activate(id)
	{
		if (this.lastId !== null)
		{
			this.getSection(this.lastId).select(false);

			let snippets = this.snippets.get(this.lastId);
			if (snippets) snippets.select(false);

			this.lastId = null;
		}

		this.getSection(id).select(true);

		let snippets = this.snippets.get(id);
		if (snippets) snippets.select(true);

		this.vizzuView.step(id);

		this.lastId = id;
	}

	getSection(id)
	{
		let sectionId = (new DocId(id)).getSectionId();
		return this.sections.get(sectionId);
	}
/*
	firstVisibleSectionId()
	{
		for (let [ id, subsection ] of this.subsections)
			if (DomHelper.isInView(subsection.element, this.contentView))
				return DomHelper.parseId(subsection.element).id;

		return null;
	}*/
}
