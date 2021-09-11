import DomHelper from "./dom-helper.js";
import SectionView from "./section-view.js";

export default class DocumentView
{
	constructor(contentElement, tocElement, document)
	{
		this.element = contentElement;
		this.tocElement = tocElement;
		this.document = document;
		this.sections = new Map();
		this.id = this.document.getOrdinal();
	}

	getSection(id) {
		let sectionId = id.split('.').slice(0, 2).join('.');
		return this.sections.get(sectionId);
	}

	populate()
	{
		let tocElement = this.addToToC()

		for (let section of this.document.sections)
		{
			let sectionView = new SectionView(
				this.element, 
				tocElement,
				section);
			
			sectionView.populate();

			this.sections.set(sectionView.id, sectionView);
		}
	}

	addToToC()
	{
		let li = 
			DomHelper.build(this.tocElement)
			.child('li', 'menuitem', this.id)
			.html(this.document.title.toUpperCase());

		let ul = li.child('ul', 'submenu', this.id)

		this.menuElement = li.element;

		return ul.element;
	}
}