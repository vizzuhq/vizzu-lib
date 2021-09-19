"use strict";

const fs = require('fs');
const jsdom = require('jsdom');
const { JSDOM } = jsdom;

const Tutorial = require('./tutorial.js');
const Examples = require('./examples.js');

class Generator
{
	constructor()
	{
		this.loadIndexHTML().then(dom => {
			this.generateTutorial(dom);
			this.generateExample(dom);
			this.saveIndexHTML(dom);
		});
	}

	loadIndexHTML()
	{
		return JSDOM.fromFile("index.in.html");
	}

	generateTutorial(dom)
	{
		let tutorial = new Tutorial();

		let view = dom.window.document.getElementById('content');
		view.innerHTML = tutorial.html;

		let toc = dom.window.document.getElementById('toc-tutorial');
		toc.innerHTML = tutorial.toc;
	}

	generateExample(dom)
	{
		let examples = new Examples();

		let view = dom.window.document.getElementById('examples-view');
		view.innerHTML = examples.html;
	}

	saveIndexHTML(dom)
	{
		fs.writeFile('../index.html', dom.serialize(), console.error);
	}
}

let run = new Generator();
