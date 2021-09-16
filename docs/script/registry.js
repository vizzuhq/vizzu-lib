class Registry
{
	constructor() 
	{
		this.snippets = new Map();
	}

	addSnippet(id, fn, options)
	{
		this.snippets.set(id, { id, fn, options });
	}
}

const registry = new Registry();