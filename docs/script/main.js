import Vizzu from '../../example/lib/vizzu.js';
import data from './sample-data.js';
import style from './example-style.js';
import documentation from './documentation.js';

function formatSnippet(snippet)
{
	return snippet
		.replace(/chart =>/, '')
		.replace(/\t/g, '  ')
		.replace(/^    /mg,' ');
}

function isInView(child, parent) {
    const childRect = child.getBoundingClientRect();
    const parentRect = parent.getBoundingClientRect();
    return childRect.top >= parentRect.top
		&& childRect.bottom <= parentRect.bottom;
}

function sectionById(id) {
	return documentation.find(section => section.id == id);
}

class Main
{
	constructor()
	{
		this.chart = new Vizzu(document.getElementById('example-canvas'));
		this.toc = document.getElementById('table-of-contents');
		this.content = document.getElementById('content');

		this.lastSection = null;
		this.nextSection = null;

		this.initializeChart();
		this.populate();

		this.contentView = document.getElementById('content-view');
		this.contentView.focus();
		this.contentView.onscroll = event => this.scrolled(event);
	}

	initializeChart()
	{
		this.anim = this.chart.initializing.then(chart =>
			chart.animate({
				data: data,
				style: style,
				config: {
					channels: {
						x: { attach: ['Timeseries'] },
						y: { attach: ['Values 1']},
						size: { attach: ['Values 1']},
					},
					title: null
				}
			})
		);
	}

	populate()
	{
		for (let section of documentation)
		{
			this.addTocItem(section);
			this.addSection(section);
		}
	}

	addTocItem(section)
	{
		let tocItem = document.createElement('li');
		tocItem.innerHTML = section.title.toUpperCase();
		tocItem.id = section.id + '-menuitem';
		tocItem.onclick = () => {
			this.scrollTo(section);
		};
		this.toc.appendChild(tocItem);
	}

	addSection(section)
	{
		const snippet = formatSnippet(section.enter.toString());

		this.content.innerHTML += `
		<h2 id="${section.id + '-title'}">${section.title}</h2>
		<div class="snippet">
			<pre><code class="JavaScript">${snippet}</code></pre>
		</div>
		<div>${section.description}</div>
		<div id="${section.id + '-end'}" class="section-end"></div>
		`;
	}

	scrolled(event)
	{
		console.log(event);
		const topSection = this.firstVisibleSection();
		if (topSection != this.lastSection) {
			this.nextSection = topSection;
			setTimeout(() => this.activate(), 500);
		}
	}

	scrollTo(section)
	{
		let sectionTitle = document.getElementById(section.id + '-title')
		sectionTitle.scrollIntoView();
	}

	activate()
	{
		if (this.nextSection == null) return;

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

		this.anim = this.anim.then(section.enter);

		let menuItem = document.getElementById(section.id + '-menuitem')
		menuItem.classList.add('toc-item-selected');

		this.lastSection = section;
	}

	firstVisibleSection()
	{
		const ends = this.content.getElementsByClassName('section-end');
		for (const end of ends)
			if (isInView(end, this.contentView))
			{
				const id = end.id.replace(/-end/, '');
				return sectionById(id);
			}
		return null;
	}
}

window.main = new Main();
