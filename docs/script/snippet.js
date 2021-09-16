
export default class Snippet
{
	constructor(element)
	{
		this.element = element;
	}

	select(selected)
	{
		if (selected) this.element.classList.add('snippet-selected');
		else  this.element.classList.remove('snippet-selected');
	}
}