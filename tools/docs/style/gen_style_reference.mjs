import puppeteer from 'puppeteer'
import path from 'path'
import serveStatic from 'serve-static'
import express from 'express'
import { fileURLToPath } from 'url'

async function createServer(workspacePath) {
	return new Promise((resolve, reject) => {
		const app = express()
		app.use(serveStatic(workspacePath))
		app.use(express.json())
		const server = app.listen(0, 'localhost', () => {
			return resolve(server)
		})
	})
}

async function createStyleReference(serverPort) {
	const browser = await puppeteer.launch({
		headless: 'new',
		args: ['--no-sandbox', '--disable-web-security']
	})
	const page = await browser.newPage()

	const style = await page.evaluate(async (serverPort) => {
		const VizzuModule = await import(
			`http://127.0.0.1:${serverPort}/docs/assets/dist/vizzu.min.js`
		)
		const Vizzu = VizzuModule.default
		const div = document.createElement('div')
		const chart = await new Vizzu(div).initializing
		const style = await chart.getComputedStyle()
		return JSON.stringify(style)
	}, serverPort)

	let content = ''
	content += `<p id="allbtn-style" class="allbtn-style"><button type="button">+&nbsp;expand all</button></p>`
	content += appendContent(JSON.parse(style), 0)
	content += `<script src="../../assets/javascripts/style_ref_allbtn.js"></script>`
	content += `<script src="../../assets/javascripts/style_ref_clickevent.js"></script>`
	console.log(content)

	browser.close()
}

function appendContent(obj, level) {
	const tab = '&emsp;'
	let content = ''
	if (level) {
		content += `<div class="collapsible-style-content">`
	}
	for (const [key, value] of Object.entries(obj)) {
		if (typeof value === 'object') {
			content += `<p class="collapsible-style"><button type="button">${tab.repeat(
				level
			)}+&nbsp;${key}</button></p>`
			content += appendContent(value, level + 1)
		} else {
			content += `<p class="not-collapsible-style">${tab.repeat(
				level
			)}${key}: <code>${value}</code></p>`
		}
	}
	if (level) {
		content += `</div>`
	}
	return content
}

const __filename = fileURLToPath(import.meta.url)
const __dirname = path.dirname(__filename)
const repoPath = __dirname + '/../../..'

const server = await createServer(repoPath)
await createStyleReference(server.address().port)
server.close()
