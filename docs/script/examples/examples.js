import DomHelper from "../dom-helper.js";

export default class Examples
{
	constructor()
	{
		this.title = document.getElementById('examples-title');
		this.title.innerHTML = 'Static Charts';

		this.examplesView = document.getElementById('examples-view'); 
		this.exampleView = document.getElementById('example-view');
		this.iframe = document.getElementById('example-frame');
		this.backButton = document.getElementById('example-back-btn');

		this.examples = new Map();

		this.discover();

		this.backButton.onclick = () => {
			this.backButton.style.display = 'none';
			this.exampleView.style.display = 'none';
			this.examplesView.style.display = 'flex';
		};

		this.iframe.addEventListener('load', () => {
			this.iframe.style.height = 
				this.iframe.contentDocument.body.scrollHeight + 50 + 'px';
			this.iframe.style.width = 
				this.iframe.contentDocument.body.scrollWidth + 'px';
		});	
	}

	discover()
	{
		let examples = document.getElementsByClassName('example');

		for (let example of examples)
		{
			let id = DomHelper.parseId(example).id;

			let thumbnail = document.getElementById('thumbnail-'+id);
			let htmlFilename = thumbnail.src.replace('png', 'html');

			example.onclick = () => {
				this.iframe.src = htmlFilename;
				this.examplesView.style.display = 'none';
				this.exampleView.style.display = 'block';
				this.backButton.style.display = 'inline-block';
			};

			this.examples.set(id, example);
		}
	}
}
