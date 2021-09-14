export default class SubSection
{
	constructor(parent)
	{
		this.parent = parent;
		this.elements = [];
	}

	p(text)
	{
		this.elements.push({ type: 'paragraph', text });
		return this;
	}

	code(title, func, options)
	{
		if (options !== undefined 
			&& options.base === undefined)
			options.base = -1;
	
		this.elements.push({ type: 'code', title, func, options });
		return this;
	}

	h() {
		return this.parent.h();
	}

	getOrdinal() {
		return this.parent.getOrdinal() + '.' + this.ordinal;
	}
}
