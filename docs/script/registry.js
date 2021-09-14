class Registry
{
	constructor() 
	{
		this.snippets = new Map();
	}

	addSnippet(id, title, fn)
	{
		this.snippets.set(id, { title, fn });
	}
}

const registry = new Registry();