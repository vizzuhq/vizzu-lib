export default class DomHelper
{
	static parseId(element)
	{
		const re = /(\w+)\-([\d\.]+)/i;
		let match = element.id.match(re);
		return { className: match[1], id: match[2] };
	}

	static isInView(child, parent) 
	{
		const childRect = child.getBoundingClientRect();
		const parentRect = parent.getBoundingClientRect();
		return childRect.top > parentRect.top - childRect.height/2;
	}
}