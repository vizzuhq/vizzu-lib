import DomHelper from "./dom-helper.js";
import Section from "./section.js";
import DocId from "./documentid.js";
import Tutorial from './tutorial/tutorial.js';

export default class Main
{
	constructor(snippetRegistry)
	{
		this.sections = new Map();

		this.discover();

		this.tutorial = new Tutorial(snippetRegistry);

		this.lastSection = null;

		this.contentView = document.getElementById('content-view');
		this.contentView.onscroll = event => this.scrolled(event);

		window.onpopstate = (event) => {
			console.log(event.state.id, this.sections.get(event.state.id).element);
			this.sections.get(event.state.id).element.scrollIntoView();
		};
		if ('scrollRestoration' in history) {
			history.scrollRestoration = 'manual';
		}
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
	}

	scrolled(event)
	{
		const topSectionId = this.firstVisibleSubtitle();
		if (topSectionId != this.lastSection)
			this.activateSection(topSectionId)
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
		const parentRect = this.contentView.getBoundingClientRect();
		return childRect.top > parentRect.top + parentRect.height/2;
	}
}
