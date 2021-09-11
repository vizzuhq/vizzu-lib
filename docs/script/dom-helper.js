class DomBuilder 
{
	constructor(element)
	{
		this.element = element;
	}

	html(html) 
	{
		this.element.innerHTML += html;
		return this;
	}

	child(tag, className, id)
	{
		let child = DomHelper.create(tag, className, id, this.element);
		return new DomBuilder(child);
	}
}

export default class DomHelper
{
	static build(element) {
		return new DomBuilder(element);
	}

	static create(tag, className, id, parent)
	{
		let element = document.createElement(tag);

		if (className !== undefined) 
		{
			element.className = className;
		
			if (id !== undefined) element.id = className + "-" + id;
		}
		
		if (parent !== undefined) parent.appendChild(element);
		
		return element;
	}

	static isInView(child, parent) 
	{
		const childRect = child.getBoundingClientRect();
		const parentRect = parent.getBoundingClientRect();
		return childRect.top > parentRect.top + 2;
	}
}