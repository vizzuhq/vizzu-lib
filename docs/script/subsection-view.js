import DomHelper from "./dom-helper.js";

export default class SubSectionView
{
	constructor(contentElement, subsection)
	{
		this.contentElement = contentElement;
		this.subsection = subsection;
		this.id = this.subsection.getOrdinal();
	}

	populate()
	{
		this.element = DomHelper.create
			('div', 'subsection', this.id, this.contentElement);
		
		for (let element of this.subsection.elements)
			this.addElement(element);

		DomHelper.build(this.element).child('div', 'subsection-end', this.id);
	}

	addElement(element)
	{
		if (element.type === 'paragraph')
			this.addParagraph(element.text);
		
		else if (element.type === 'code')
			this.addSnippet(element.title, element.func);
	}

	addParagraph(text)
	{
		DomHelper.build(this.element)
			.child('div', 'paragraph', this.id)
			.html(text);
	}

	addSnippet(title, func)
	{
		DomHelper.build(this.element)
			.child('div', 'snippet', this.id)
			.child('pre')
			.child('code', 'JavaScript', this.id)
			.html(this.functionToString(func));
	}

	functionToString(func)
	{
		return func.toString()
			.replace(/chart =>\s*\n/, '')
			.replace(/\t/g, '  ')
			.replace(/^  /mg,' ');
	}
}