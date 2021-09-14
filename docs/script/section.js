
export default class Section
{
	constructor(element)
	{
		this.element = element;
	}

	setMenu(element)
	{
		this.menuElement = element;

		this.menuElement.onclick = () => {
			this.element.scrollIntoView();
		};
	}

	select(selected)
	{
		let list = this.menuElement.classList;
		if (selected) list.add('submenuitem-selected');
		else list.remove('submenuitem-selected');
	}

}