import DomHelper from "./dom-helper.js";

export default class SideBar
{
	constructor()
	{
		this.pages = new Map();

		this.discover();
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