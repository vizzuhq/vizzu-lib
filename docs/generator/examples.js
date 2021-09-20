"use strict";

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');
const Example = require('./example.js');

class Examples
{
	constructor()
	{
		this.inputPath = '../../test/integration/test_report/web_content/';
		this.staticsPath = this.inputPath + 'sample_static/';
		this.animationsPath = this.inputPath + 'templates/';

		this.statics = this.collectStatics();
		this.moveToDocs(this.statics, 'sample_static', 'static');

		this.generateHtml(this.statics);

		this.generateHtmls(this.statics);

//		this.collectAnimations();
	}

	generateHtml(examples)
	{
		let index = 0;
		this.html = '';
		for (let example of examples)
		{
			let id = `1.0.${index}`; 
			this.html += `
			<div class="example" id="example-${id}">
				<img class="thumbnail" id="thumbnail-${id}" src="${example.urlBase}.png" />
			</div>
			`;
			index++;
		}
	}

	generateHtmls(examples)
	{
		for (let example of examples)
		{
			new Example(example);
		}
	}

	moveToDocs(examples, sourcedir, targetdir)
	{
		let targetBase = 'content/examples/';
		var re = new RegExp('.*'+sourcedir);
		for (let example of examples)
		{
			let pngSource = example.filename;
			let target = path.dirname(pngSource);
			target = target.replace(re,targetdir);
			
			target = targetBase+target;
			example.urlBase = target;

			target = '../'+target;

			example.targetBasename = target; 
			let targetPng = target +'.png';
			let targetJs = target +'.js';

			let dirname = path.dirname(targetPng);
			if (!fs.existsSync(dirname)) 
				fs.mkdirSync(dirname, { recursive: true });
			
			sharp(pngSource)
				.resize(320)
				.toFile(targetPng, (err, info) => { if(err) console.error(err); });

			let jsSource = pngSource.replace('test_report', 'test_cases');
			example.jsSource = path.dirname(jsSource) + '.mjs';
		}
	}

	collectStatics()
	{
		let list = this.getAllFiles(this.staticsPath);
		list = list.filter(filename => filename.match(/100.000%.png$/));
		list = list.map(filename => { return { 
			filename, 
			type: 'static'
		}});
		return list;
	}

	collectAnimations()
	{
		let list = this.getAllFiles(this.animationsPath);
		list = list.filter(filename => filename.match(/100.000%.png$/));
		console.log('list:');
		console.log(list);
	}

	getAllFiles(dirPath, arrayOfFiles) 
	{
		let files = fs.readdirSync(dirPath)

		arrayOfFiles = arrayOfFiles || []
	  
		files.forEach(file => {
			if (fs.statSync(dirPath + "/" + file).isDirectory()) 
			{
				arrayOfFiles = this.getAllFiles(dirPath + "/" + file, arrayOfFiles)
			} else 
			{
				arrayOfFiles.push(path.join(__dirname, dirPath, "/", file))
			}
		})
  
		return arrayOfFiles
	}
}

module.exports = Examples;