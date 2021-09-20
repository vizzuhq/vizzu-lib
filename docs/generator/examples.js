"use strict";

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');
const Example = require('./example.js');

class Examples
{
	constructor()
	{
		this.examples = [];
		this.examples.push(...this.collect('sample_static', 'static'));
		this.examples.push(...this.collect('templates', 'animated'));

		this.moveToDocs();
		this.generateHtml();
		this.generateHtmls();
	}

	generateHtml()
	{
		let index = 0;
		this.html = '';
		for (let example of this.examples)
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
		for (let example of this.examples)
		{
			new Example(example);
		}
	}

	moveToDocs()
	{
		let targetBase = 'content/examples/';

		for (let example of this.examples)
		{
			var re = new RegExp('.*'+example.inputFolder);

			let target = targetBase + example.outputFolder + '/'
				+ path.basename(example.jsFilename, '.mjs');

			example.urlBase = target;

			target = '../'+target;

			example.targetBasename = target; 

			let targetPng = example.targetBasename +'.png';

			let dirname = path.dirname(targetPng);
			if (!fs.existsSync(dirname)) 
				fs.mkdirSync(dirname, { recursive: true });
			
			sharp(example.pngFilename)
				.resize(320)
				.toFile(targetPng, (err, info) => { if(err) console.error(err); });
		}
	}

	collect(inputFolder, outputFolder)
	{
		let inputPath = '../../test/integration/test_cases/web_content/';
		let jsFolder = inputPath + inputFolder + '/';
		let list = this.getAllFiles(jsFolder);
		list = list.filter(filename => filename.match(/.mjs$/));
		list = list.map(jsFilename => 
		{
			let basename = path.basename(jsFilename, '.mjs');
			let pngFilename = jsFilename
				.replace('test_cases', 'test_report')
				.replace('.mjs', `/${basename}_000_100.000%.png`);

			return { jsFilename, pngFilename, inputFolder, outputFolder };
		});
		return list;
	}

	getAllFiles(dirPath, arrayOfFiles) 
	{
		let files = fs.readdirSync(dirPath)

		arrayOfFiles = arrayOfFiles || [];
	  
		files.forEach(file => {
			if (fs.statSync(dirPath + "/" + file).isDirectory()) 
			{
				arrayOfFiles = this.getAllFiles(dirPath + "/" + file, arrayOfFiles)
			} else 
			{
				arrayOfFiles.push(path.join(__dirname, dirPath, "/", file))
			}
		})
  
		return arrayOfFiles;
	}
}

module.exports = Examples;