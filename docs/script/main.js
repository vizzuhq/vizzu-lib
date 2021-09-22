import Tutorial from './tutorial/tutorial.js';
import Examples from './examples/examples.js';
import DomHelper from "./dom-helper.js";

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
			console.log(event.state.id, this.tutorial.sections.get(event.state.id).element);
			this.tutorial.sections.get(event.state.id).element
				.scrollIntoView({ behavior: 'auto' });
		};
		if ('scrollRestoration' in history) {
			history.scrollRestoration = 'manual';
		}

		this.tutorial.gotoInitSection();
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
