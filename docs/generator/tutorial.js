"use strict";

const fs = require('fs');
const marked = require('marked');

class Tutorial
{
	constructor()
	{
		this.toc = '';
		this.html = '';
		this.divOpened = false;

		this.section = -1;
		this.subsection = -1;

		this.setupMarked();
		let markedContent = this.readMarkedFiles();

		this.html = marked(markedContent) + '</div>\n\n';
	}

	setupMarked()
	{
		marked.setOptions({
			highlight: function(code, lang) {
			  const hljs = require('highlight.js');
			  const language = hljs.getLanguage(lang) ? lang : 'plaintext';
			  return hljs.highlight(code, { language }).value;
			}
		});
		
		this.defaultRender = new marked.Renderer();

		marked.use(
		{
			renderer: {
				heading: this.heading.bind(this),
				paragraph: this.paragraph.bind(this),
				code: this.code.bind(this),
			}
		});
	}

	readMarkedFiles()
	{
		let content = '';

		let dir = '../content/tutorial';
		let files = fs.readdirSync(dir);
		
		files = files.filter(name => name.match(/\d+/));
		files.sort((a, b) => {
			return Number(a.match(/\d+/)[0]) < Number(b.match(/\d+/)[0]) ? -1 : 1;
		});
		
		files.forEach(file => {
			if (file.match(/\d+.*\.md/)) 
				content += '\n\n' + fs.readFileSync(dir + '/' + file); 
		});

		return content;
	}

	heading(text, level, raw, slugger)
	{
		let res = '';
		if (this.divOpened)
		{
			res += '</div>\n\n';
			this.divOpened = false;
		}
		if (level === 2) 
		{
			this.divOpened = true;
			this.section++;
			this.subsection = 0;
			this.toc += `<li><a href="#chapter-0.${this.section}">${text}</a></li>
			`;
			res += `
			<div data-title="${text}">
				<a class="anchor" id="chapter-0.${this.section}"></a>
				<h2 class="subtitle">${text}</h2>
			`;
			return res;
		}
		else {
			res += this.defaultRender.heading(text, level, raw, slugger);
			return res;
		}
	}

	paragraph(text) {
		return `<p class="paragraph">${text}</p>`
	}

	code(code, infostring, escaped)
	{
		let info = this.parseInfostring(infostring);

		let snippet = this.defaultRender.code(code, info.lang, escaped);

		let idAttr = '';
		let script = '';
		let tabindex = '';
		let classNames = 'snippet';
		let snippetPlayer = '';

		if (info.lang === 'javascript')
		{
			let codeInject;
			
			if (info.options.run)
			{
				this.subsection++;
				let id = `0.${this.section}.${this.subsection}`;
				idAttr = `id='snippet-${id}'`;
				classNames += ' runable';
				tabindex = 'tabindex="0"';

				let  codeToRegister = code;
				if (codeToRegister.match(/^\s*chart.animate\(/))
					codeToRegister = codeToRegister
						.replace(/\s*chart.animate\(/, '')
						.replace(/\)\s*$/, '');
				else 
					codeToRegister = `chart => { ${codeToRegister}; return chart; }`;

				codeInject = `
					registry.addSnippet(
						${JSON.stringify(id)}, 
						${codeToRegister},
						${JSON.stringify(info.options)});
				`;

				snippetPlayer = `
				<div class="snippet-player" data-snippet-id="${id}">
					<img class="snippet-player-snapshot" alt="snapshot" src="content/snapshot/snippet.png" />
				</div>
				`;
			}

			if (info.options.pure) codeInject = code;

			if (info.options.run || info.options.pure)
				script = `<script>${codeInject}</script>`;
		}

		let res = `
		<div ${idAttr} class='${classNames}' ${tabindex}>
			${snippetPlayer}
			${snippet}
			${script}
		</div>
		`;
		return res;
	}

	parseInfostring(infostring)
	{
		let match = infostring.match(/^(\w+)(\s+.*)*/);
		let lang = match[1];
		let options = match[2] === undefined ? undefined
			: JSON.parse(match[2]);

		if (options === undefined) options = {};
		if (options.run === undefined) options.run = true;
		if (options.pure === undefined) options.pure = false;

		return { lang, options };
	}
}

module.exports = Tutorial;