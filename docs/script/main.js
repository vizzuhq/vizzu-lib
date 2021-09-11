import tutorial from '../content/tutorial/tutorial.js';
import DocumentView from './document-view.js';
import DomHelper from './dom-helper.js';
import VizzuView from './vizzu-view.js';

class Main
{
	constructor()
	{
		this.vizzuView = new VizzuView('example-canvas');
		this.toc = document.getElementById('table-of-contents');
		this.content = document.getElementById('content');

		this.lastSection = null;
		this.nextSection = null;

		this.populate();

		this.contentView = document.getElementById('content-view');
		this.contentView.focus();
		this.contentView.onscroll = event => this.scrolled(event);
	}

	populate()
	{
		tutorial.ordinal = 0;
		this.documentView = new DocumentView(this.content, this.toc, tutorial);
		this.documentView.populate();
	}

	scrolled(event)
	{
		const topSection = this.firstVisibleSection();
		if (topSection != this.lastSection) {
			this.nextSection = topSection;
			console.log(this.nextSection);
			setTimeout(() => this.activate(), 500);
		}
	}

	activate()
	{
		if (this.nextSection == null) return;
/*
		const section = this.nextSection;
		this.nextSection = null;

		if (this.lastSection !== null)
		{
			this.anim = this.anim.then(this.lastSection.leave);
			let menuItem = document.getElementById(this.lastSection.id + '-menuitem')
			menuItem.classList.remove('toc-item-selected');
			menuItem.classList.add('toc-item');
			this.lastSection = null;
		}

		this.vizzuView.step(section.enter);

		let menuItem = document.getElementById(section.id + '-menuitem')
		menuItem.classList.add('toc-item-selected');

		this.lastSection = section;
	*/	}

	firstVisibleSection()
	{
		const ends = this.content.getElementsByClassName('subsection-end');
		for (const end of ends)
			if (DomHelper.isInView(end, this.contentView))
				return end.id.replace(/-end/, '');

		return null;
	}
}

window.main = new Main();
