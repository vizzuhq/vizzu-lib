export default class DomHelper
{
	static parseIdString(idString)
	{
		const re = /(\w+)\-([\d\.]+)/i;
		let match = idString.match(re);
		return { className: match[1], id: match[2] };
	}

	static parseId(element)
	{
		return DomHelper.parseIdString(element.id);
	}
}