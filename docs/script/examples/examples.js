import DomHelper from "../dom-helper.js";
import DocId from "../documentid.js";

export default class Examples
{
	constructor()
	{
		this.actSubpage = '1.0';

		this.page = document.getElementById('page-1');
		this.title = document.getElementById('examples-title');
		this.title.innerHTML = 'Static Charts';

		this.examplesView = new Map();
		this.exampleView = document.getElementById('example-view');
		this.iframe = document.getElementById('example-frame');
		this.backButton = document.getElementById('example-back-btn');

		this.examples = new Map();

		this.discover();

		this.backButton.onclick = () => { this.closeExample(); };

		this.iframe.addEventListener('load', () => {
			this.iframe.style.height = 
				this.iframe.contentDocument.body.scrollHeight + 50 + 'px';
			this.iframe.style.width = 
				this.iframe.contentDocument.body.scrollWidth + 'px';
		});

		this.iframe.onload = () =>
		{
			this.iframe.contentWindow.document.onkeydown = (e) => {
				if (e.which === 27 || e.which === 8) this.closeExample();
			};	
		}
	}

	closeExample()
	{
		this.selectSubPage(this.actSubpage);
	}

	navigateToId(id, pushState)
	{
		console.log(id);
		let docId = new DocId(id);
		let sectionId = docId.getSectionId();

		this.selectSubPage(sectionId, pushState);

		if (docId.subsection !== undefined)
			this.selectExample(id, pushState);
	}

	selectSubPage(selectedId, pushState = true)
	{
		this.actSubpage = selectedId;

		this.title.innerHTML = 
			this.actSubpage == '1.0' ? 'Static Charts' :
			this.actSubpage == '1.1' ? 'Animated Charts': '';

		this.backButton.style.display = 'none';
		this.exampleView.style.display = 'none';
	
		for (let [id, view] of this.examplesView)
		{
			view.style.display = id == selectedId
				? 'flex' : 'none';
		}

		this.examples.get(selectedId+'.0').focus();

		if (pushState)
			history.pushState({ id: selectedId }, '', `#examples-${selectedId}`);
	}

	discover()
	{
		let exampleViews = document.getElementsByClassName('examples');
		for (let view of exampleViews)
		{
			let id = DomHelper.parseId(view).id;
			this.examplesView.set(id, view);
		}

		let examples = document.getElementsByClassName('example');

		for (let example of examples)
		{
			let id = DomHelper.parseId(example).id;

			example.onclick = () => { this.selectExample(id); };

			example.onkeydown = (e) => {
				if (e.which === 13) this.selectExample(id);
			};

			this.examples.set(id, example);
		}

		let submenus = document.getElementsByClassName('submenuitem');
		for (let submenu of submenus)
		{
			const id = DomHelper.parseId(submenu).id;
			if ((new DocId(id)).document == 1)
				submenu.onclick = () => {
					this.onMenu(id);
					this.selectSubPage(id);
				}
		}
	}

	selectExample(id, pushState = true)
	{
		let thumbnail = document.getElementById('thumbnail-'+id);
		let htmlFilename = '';

		if (thumbnail.tagName === 'IMG')
		{
			htmlFilename = thumbnail.src.replace('png', 'html');
		}
		else
		{
			htmlFilename = thumbnail.getElementsByTagName("source")[0]
				.src.replace('webm','html');
		}

		this.page.scroll(0,0);
		this.iframe.src = htmlFilename;
		for (let [id, view] of this.examplesView)
			view.style.display = 'none';
		this.exampleView.style.display = 'block';
		this.backButton.style.display = 'inline-block';

		this.iframe.focus();

		if (pushState) history.pushState({ id }, '', `#example-${id}`)
	}
}
