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
		this.subtitle = null;

		const renderer = {
			code: (code, infostring, escaped) =>
			{
				let id = `${this.section}.${this.subsection+1}`;
				let res = markedRender.code(code, infostring, escaped) + `
				<script>
					registerCode(${JSON.stringify(id)}, ${JSON.stringify(this.subtitle)}, chart => { ${code} });
				</script>
				`;
				this.subtitle = null;
				return res;
			},
			blockquote: (quote) =>
			{
				this.subsection++;
				return `
				<div id='subsection-${this.section}.${this.subsection}' class="subsection">
					${quote}
				</div>
				`;
			},
			heading: (text, level, raw, slugger) =>
			{
				if (level === 3) {
					this.subtitle = text;
					return '';
				}
				else if (level === 2) {
					this.section++;
					this.subsection = -1;
					this.toc += `
						<li id="menu-${this.section}">${text}</li>`;
					return `
					<h2 id="section-${this.section}" class="section">${text}</h2>
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
		let contentView = this.dom.window.document.getElementById('content-view');
		contentView.innerHTML = marked(this.content);

		let toc = this.dom.window.document.getElementById('toc-tutorial');
		toc.innerHTML = this.toc;

		fs.writeFile('../index.html', this.dom.serialize(), err => console.error);
	}
}

let run = new Generator();
