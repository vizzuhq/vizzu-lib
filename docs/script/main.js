import Tutorial from './tutorial/tutorial.js';
import Examples from './examples/examples.js';
import DomHelper from "./dom-helper.js";

export default class Main
{
	constructor(snippetRegistry)
	{
		this.pages = new Map();

		this.discover();

		this.tutorial = new Tutorial(snippetRegistry);

		this.examples = new Examples();

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

		let menuItems = document.getElementsByClassName('menuitem');
		for (let menuItem of menuItems)
		{
			let id = DomHelper.parseId(menuItem).id;

			menuItem.onclick = () => {
				let page = this.pages.get(id);
				this.setActivePage(page);
			}
		}
	}

	setActivePage(pageToActivate)
	{
		for (let [id, page] of this.pages)
		{
			page.style.display = page == pageToActivate
				? 'block' : 'none';
		}
	}
}
