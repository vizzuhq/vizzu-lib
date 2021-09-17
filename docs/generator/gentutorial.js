const fs = require('fs');
const jsdom = require('jsdom');
const marked = require('marked');
const { JSDOM } = jsdom;

class Generator
{
	constructor()
	{
		marked.setOptions({
			highlight: function(code, lang) {
			  const hljs = require('highlight.js');
			  const language = hljs.getLanguage(lang) ? lang : 'plaintext';
			  return hljs.highlight(code, { language }).value;
			}
		});
		
		let markedRender = new marked.Renderer();

		this.section = -1;
		this.subsection = -1;
		this.toc = '';

		const renderer = {
			code: (code, infostring, escaped) =>
			{
				let match = infostring.match(/^(\w+)(\s+.*)*/);
				infostring = match[1];
				let options = match[2] === undefined ? undefined
					: JSON.parse(match[2]);

				let snippet = markedRender.code(code, infostring, escaped);

				let isJS = infostring === 'javascript';

				if (options === undefined) options = {};
				if (options.run === undefined) options.run = true;
				if (options.pure === undefined) options.pure = false;

				let idAttr = '';
				let script = '';
				let tabindex = '';
				let classNames = 'snippet';

				if (isJS)
				{
					let codeInject;
					
					if (options.run)
					{
						this.subsection++;
						let id = `0.${this.section}.${this.subsection}`;
						idAttr = `id='snippet-${id}'`;
						classNames += ' runable';
						tabindex = 'tabindex="0"';

						codeInject = `
							registry.addSnippet(
								${JSON.stringify(id)}, 
								chart => { ${code} },
								${JSON.stringify(options)});
						`;
					}

					if (options.pure) codeInject = code;
	
					if (options.run || options.pure)
						script = `<script>${codeInject}</script>`;
				}

				let res = `
				<div ${idAttr} class='${classNames}' ${tabindex}>
					${snippet}
					${script}
				</div>
				`;
				return res;
			},
			paragraph: (text) => {
				return `
				<p class="paragraph">${text}</p>`
			},
			heading: (text, level, raw, slugger) =>
			{
				if (level === 2) 
				{
					this.section++;
					this.subsection = 0;
					this.toc += `
						<li id="submenuitem-0.${this.section}" class="submenuitem">${text}</li>`;
					return `
					<h2 id="subtitle-0.${this.section}" class="subtitle">${text}</h2>
					`;
				}
				else return markedRender.heading(text, level, raw, slugger);
			}
		};
		
		marked.use({ renderer });
		
		this.indexHTMLReady = JSDOM.fromFile("index.in.html");
		
		this.indexHTMLReady.then(dom => {
			this.document = dom.window.document;
			this.dom = dom;
			this.readMds();
			this.write();
		});
	}

	readMds()
	{
		this.content = '';

		let dir = '../content/tutorial';
		let files = fs.readdirSync(dir);
		
		files = files.filter(name => name.match(/\d+/));
		files.sort((a, b) => {
			return Number(a.match(/\d+/)[0]) < Number(b.match(/\d+/)[0]) ? -1 : 1;
		});
		
		files.forEach(file => {
			if (file.match(/\d+.*\.md/)) 
				this.content += '\n\n' + fs.readFileSync(dir + '/' + file); 
		});
	}

	write() 
	{
		let contentView = this.dom.window.document.getElementById('content');
		contentView.innerHTML = marked(this.content);

		let toc = this.dom.window.document.getElementById('toc-tutorial');
		toc.innerHTML = this.toc;

		fs.writeFile('../index.html', this.dom.serialize(), err => console.error);
	}
}

let run = new Generator();
