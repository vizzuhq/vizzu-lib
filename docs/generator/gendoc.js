"use strict";

const fs = require('fs');
const jsdom = require('jsdom');
const { JSDOM } = jsdom;

const Tutorial = require('./tutorial.js');

class Generator
{
	constructor()
	{		
		this.loadIndexHTML().then(dom => {
			this.generateTutorial(dom);
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

		let contentView = dom.window.document.getElementById('content');
		contentView.innerHTML = tutorial.html;

		let toc = dom.window.document.getElementById('toc-tutorial');
		toc.innerHTML = tutorial.toc;
	}

	saveIndexHTML(dom)
	{
		fs.writeFile('../index.html', dom.serialize(), console.error);
	}
}

let run = new Generator();
