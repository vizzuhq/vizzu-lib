import SubSection from './subsection.js';

export default class Section 
{
	constructor(title, parent)
	{
		this.title = title;
		this.parent = parent;
		this.subSections = [];
	}

	p(text)
	{
		return this.h().p(text);
	}

	code(title, func)
	{
		return this.h('').code(title, func);
	}

	h()
	{
		let newSubSection = new SubSection(this);
		newSubSection.ordinal = this.subSections.length; 
		this.subSections.push(newSubSection);
		return newSubSection;
	}

	getOrdinal() {
		return this.parent.getOrdinal() + '.' + this.ordinal;
	}
}