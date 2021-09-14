
export default class SubSection
{
	constructor(element)
	{
		this.element = element;
	}

	setSnippet(snippet)
	{
		this.snippetElement = snippet;

		this.snippetElement.onclick = () => {
			this.element.scrollIntoView();
		};
	}

	select(selected)
	{
		if (selected) 
		{
			this.element.classList.add('subsection-selected');
			if (this.snippetElement !== undefined)
				this.snippetElement.classList.add('snippet-selected');
		}
		else 
		{
			this.element.classList.remove('subsection-selected');
			if (this.snippetElement !== undefined)
				this.snippetElement.classList.remove('snippet-selected');
		}
	}

}