import DocId from "./documentid.js";
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
		let sectionId = (new DocId(id)).getSectionId();
		return this.sections.get(sectionId);
	}

	visitSections(visitor) {
		for (let [id, section] of this.sections)
			visitor(section);
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