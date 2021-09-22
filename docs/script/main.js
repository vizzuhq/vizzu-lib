import Tutorial from './tutorial/tutorial.js';
import Examples from './examples/examples.js';
import DomHelper from "./dom-helper.js";
import DocId from './documentid.js';

export default class Main
{
	constructor(snippetRegistry)
	{
		this.pages = new Map();
		this.menus = new Map();

		this.discover();

		this.tutorial = new Tutorial(snippetRegistry);
		this.tutorial.sectionScrolledIn = id => {
			this.activateSubMenu(id);
		};
		this.tutorial.onMenu = id => {
			this.setActivePage('0');
		};

		this.examples = new Examples();
		this.examples.onMenu = id => { 
			this.setActivePage('1');
			this.activateSubMenu(id);
		};

		document.getElementById('menuitem-2').onclick = () =>
		{
			window.open("reference/index.html");
		}

		window.onpopstate = (event) => {
			this.navigateToId(event.state.id);
		};
		if ('scrollRestoration' in history) {
			history.scrollRestoration = 'manual';
		}

		let initId = '0.0.0';
		let hash = window.location.hash;
		if (hash) initId = DomHelper.parseIdString(hash).id;
		this.navigateToId(initId);
	}

	navigateToId(id)
	{
		let documentId = (new DocId(id)).document;
		this.setActivePage(documentId);
		if (documentId === 0) this.tutorial.navigateToId(id);
		else if (documentId === 1) this.examples.navigateToId(id, false);
	}

	discover()
	{
		let pages = document.getElementsByClassName('page');
		for (let page of pages)
		{
			let id = DomHelper.parseId(page).id;
			this.pages.set(id, page);
		}

		let submenus = document.getElementsByClassName('submenuitem');
		for (let submenu of submenus)
		{
			const id = DomHelper.parseId(submenu).id;
			this.menus.set(id, submenu);
		}
	}

	setActivePage(pageId)
	{
		for (let [id, page] of this.pages)
		{
			if (id == pageId) page.style.display = 'block';
			else page.style.display = 'none';
		}
	}

	activateSubMenu(activeId)
	{
		for (let [id, menu] of this.menus)
		{
			let list = menu.classList;
			if (id == activeId) list.add('submenuitem-selected');
			else list.remove('submenuitem-selected');
		}
	}
}
