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

	code(title, func)
	{
		this.elements.push({ type: 'code', title, func });
		return this;
	}

	h() {
		return this.parent.h();
	}

	getOrdinal() {
		return this.parent.getOrdinal() + '.' + this.ordinal;
	}
}
