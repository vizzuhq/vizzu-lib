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

		let view0 = dom.window.document.getElementById('examples-1.0');
		view0.innerHTML = examples.htmlStatic;

		let view1 = dom.window.document.getElementById('examples-1.1');
		view1.innerHTML = examples.htmlAnimated;
	}

	saveIndexHTML(dom)
	{
		fs.writeFile('../index.html', dom.serialize(), console.error);
	}
}

let run = new Generator();
