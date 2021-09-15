import DomHelper from "./dom-helper.js";
import Section from "./section.js";
import SubSection from "./subsection.js";
import DocId from "./documentid.js";
import VizzuView from './vizzu-view.js';

export default class Main
{
	constructor(snippetRegistry)
	{
		this.sections = new Map();
		this.subsections = new Map();

		this.discover();

		this.setupVizzu(snippetRegistry);

		this.navigation = {
			lastId: null,
			nextId: null,
			currentMove: 0
		};

		this.contentView = document.getElementById('content-view');
		this.contentView.focus();
		this.contentView.onscroll = event => this.scrolled(event);
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

		let subsections = document.getElementsByClassName('subsection');
		for (let subsection of subsections)
		{
			const id = DomHelper.parseId(subsection).id;
			this.subsections.set(id, new SubSection(subsection));
		}

		let snippets = document.getElementsByClassName('snippet');
		for (let snippet of snippets)
		{
			const id = DomHelper.parseId(snippet).id;
			this.subsections.get(id).setSnippet(snippet);
		}
	}

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

	activate()
	{
		if (this.navigation.nextId == null) return;

		if (this.navigation.lastId !== null)
		{
			this.getSection(this.navigation.lastId).select(false);

			let subsection = this.subsections.get(this.navigation.lastId);
			if (subsection) subsection.select(false);

			this.navigation.lastId = null;
		}

		this.getSection(this.navigation.nextId).select(true);

		let subsection = this.subsections.get(this.navigation.nextId);
		if (subsection) subsection.select(true);

		this.vizzuView.step(this.navigation.nextId);

		this.navigation.lastId = this.navigation.nextId;
		this.navigation.nextId = null;
	}

	getSection(id)
	{
		let sectionId = (new DocId(id)).getSectionId();
		return this.sections.get(sectionId);
	}

	firstVisibleSectionId()
	{
		for (let [ id, subsection ] of this.subsections)
			if (DomHelper.isInView(subsection.element, this.contentView))
				return DomHelper.parseId(subsection.element).id;

		return null;
	}
}
