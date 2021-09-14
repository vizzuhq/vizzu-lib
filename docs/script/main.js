import tutorial from '../content/tutorial/tutorial.js';
import DocumentView from './document-view.js';
import DomHelper from './dom-helper.js';
import VizzuView from './vizzu-view.js';
class Main
{
	constructor()
	{
		this.vizzuView = new VizzuView('example-canvas', tutorial.data);
		this.toc = document.getElementById('table-of-contents');
		this.content = document.getElementById('content');

		this.lastSection = null;
		this.nextSection = null;
		this.currentId = 0;

		this.populate();

		this.contentView = document.getElementById('content-view');
		this.contentView.focus();
		this.contentView.onscroll = event => this.scrolled(event);
	}

	populate()
	{
		this.documentView = new DocumentView(this.content, this.toc, tutorial);
		this.documentView.populate();
		this.documentView.visitSections(section => {
			section.visitSubSections(subSection => {
				if (subSection.code !== undefined)
					this.vizzuView.register(subSection.id, subSection.code);
			});
		});
	}

	scrolled(event)
	{
		const topSection = this.firstVisibleSectionId();
		if (topSection != this.lastSection) {
			this.nextSection = topSection;
			console.log(this.nextSection);
			this.currentId++;
			let id = this.currentId;
			setTimeout(() => {
				if (this.currentId === id) this.activate()
			}, 300);
		}
	}

	activate()
	{
		if (this.nextSection == null) return;

		const section = this.nextSection;
		this.nextSection = null;

		if (this.lastSection !== null)
		{
			let sectionView = this.documentView.getSection(this.lastSection);
			sectionView.select(false);
			
			let subsectionView = sectionView.getSubSection(this.lastSection);
			
			subsectionView.select(false);

			this.lastSection = null;
		}

		let sectionView = this.documentView.getSection(section);
		sectionView.select(true);

		let subsectionView = sectionView.getSubSection(section);
		subsectionView.select(true);

		if (subsectionView.code !== undefined)
		{
			this.vizzuView.step(subsectionView.id);
		}

		this.lastSection = section;
	}

	firstVisibleSectionId()
	{
		const ends = this.content.getElementsByClassName('subsection-end');
		for (const end of ends)
			if (DomHelper.isInView(end, this.contentView))
				return DomHelper.parseId(end).id;

		return null;
	}
}

window.main = new Main();
