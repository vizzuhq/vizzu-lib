import SubSectionView from "./subsection-view.js";
import DomHelper from "./dom-helper.js";

export default class SectionView
{
	constructor(contentElement, tocElement, section)
	{
		this.contentElement = contentElement;
		this.tocElement = tocElement;
		this.section = section;
		this.subSections = new Map();
		this.id = this.section.getOrdinal();
	}

	getSubSection(id)
	{
		return this.subSections.get(id);
	}

	populate()
	{
		this.element = DomHelper.create
			('div', 'section', this.id, this.contentElement);

		this.addTitle();

		for (let subsection of this.section.subSections)
		{
			let subSectionView = new SubSectionView(
				this.element, 
				subsection);
			
			subSectionView.populate();

			this.subSections.set(subSectionView.id, subSectionView);
		}

		this.addToToC();
	}

	addToToC()
	{
		this.menuElement = DomHelper.create
			('li', 'submenuitem', this.id, this.tocElement);

		this.menuElement.innerHTML = this.section.title;

		this.menuElement.onclick = () => {
			this.element.scrollIntoView();
		};
	}

	addTitle()
	{
		DomHelper.build(this.element)
			.child('h2', 'subtitle', this.id)
			.html(this.section.title);
	}

	select(selected)
	{
		let list = this.menuElement.classList;
		if (selected) list.add('submenuitem-selected');
		else list.remove('submenuitem-selected');
	}

}