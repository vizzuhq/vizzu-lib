import Section from './section.js'

export default class Document
{
	constructor(title)
	{
		this.title = title;
		this.sections = [];
	}

	section(title) {
		let newSection = new Section(title, this);
		newSection.ordinal = this.sections.length; 
		this.sections.push(newSection);
		return newSection;
	}

	getOrdinal() {
		return this.ordinal;
	}
}