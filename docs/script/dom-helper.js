export default class DomHelper
{
	static parseId(element)
	{
		const re = /(\w+)\-([\d\.]+)/i;
		let match = element.id.match(re);
		return { className: match[1], id: match[2] };
	}
}