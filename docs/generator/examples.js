"use strict";

const fs = require('fs');
const path = require('path');
const sharp = require('sharp');
const Example = require('./example.js');
const exampleList = require('./example-list.js');

class Examples
{
	constructor()
	{
		this.examples = [];
		this.examples.push(...this.collect('sample_static', 'static', 'png'));
		this.examples.push(...this.collect('templates', 'animated', 'webm'));

		this.moveToDocs();
		this.generateHtml();
		this.generateHtmls();
	}

	generateHtml()
	{
		let indexStatic = 0;
		let indexAnimated = 0;
		this.htmlStatic = '';
		this.htmlAnimated = '';
		for (let example of this.examples)
		{
			if (example.outputFolder === 'static')
			{
				let id = `1.0.${indexStatic}`;
				this.htmlStatic += `
					<div class="example" id="example-${id}" tabindex="0">
						<img class="thumbnail" id="thumbnail-${id}" src="${example.urlBase}.png" />
					</div>
				`;
				indexStatic++;
			}
			else
			{
				let id = `1.1.${indexAnimated}`;
				this.htmlAnimated += `
					<div class="example" id="example-${id}" tabindex="0">
						<video class="thumbnail" id="thumbnail-${id}" width="320" height="180" nocontrols autoplay muted loop>
							<source src="${example.urlBase}.webm" type="video/webm">
							<source src="${example.urlBase}.mp4" type="video/mp4">
							Your browser does not support the video tag.
						</video>
					</div>
				`;
				indexAnimated++;
			}
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
			let target = targetBase + example.outputFolder + '/'
				+ path.basename(example.jsFilename, '.mjs');

			example.urlBase = target;

			target = '../'+target;

			example.targetBasename = target; 

			let thumbnail = example.targetBasename +'.' + example.extension;

			let dirname = path.dirname(thumbnail);
			if (!fs.existsSync(dirname)) 
				fs.mkdirSync(dirname, { recursive: true });

			if (example.outputFolder === 'static')
			{				
				sharp(example.pngFilename)
					.resize(320)
					.toFile(thumbnail, (err, info) => { if(err) console.error(err); });	
			}
			else
			{
				fs.copyFileSync(example.webmFilename, example.targetBasename+'.webm');
				fs.copyFileSync(example.mp4Filename, example.targetBasename+'.mp4');
			}
		}
	}

	collect(inputFolder, outputFolder, extension)
	{
		let inputPath = '../../test/integration/test_cases/web_content/';
		let jsFolder = inputPath + inputFolder + '/';
		let list = Object.keys(exampleList[inputFolder]);
		return list
			.filter(filename => filename.match(/.mjs$/))
			.map(jsFilename => jsFolder + jsFilename)
			.map(jsFilename => 
			{
				let basename = path.basename(jsFilename, '.mjs');
				
				let pngFilename = jsFilename
					.replace('test_cases', 'test_report')
					.replace('.mjs', `/${basename}_000_100.000%.png`);

				let webmFilename = jsFilename
				.replace('test_cases/web_content/templates/', 
					'modules/videorecorder/resized/web_content_templates_')
				.replace('.mjs', '.webm');

				let mp4Filename = webmFilename.replace('.webm', '.mp4');

				return { 
					jsFilename, 
					pngFilename,
					webmFilename,
					mp4Filename,
					outputFolder,
					extension
				};
			});
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