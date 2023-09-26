import fs from 'fs/promises'
import puppeteer from 'puppeteer'
import path from 'path'
import { fileURLToPath } from 'url'

const __filename = fileURLToPath(import.meta.url)
const __dirname = path.dirname(__filename)

const repoPath = __dirname + '/../../..'
const mkdocsPath = `${repoPath}/tools/docs`
const genPath = `${mkdocsPath}/style`
const jsAssetsPath = '../../assets/javascripts'

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

const Vizzu = process.argv[2]

const browserLaunched = puppeteer.launch({ headless: 'new' })

const pageCreated = browserLaunched.then((browser) => {
  return browser.newPage()
})

const getStyleScriptLoaded = fs.readFile(`${genPath}/get_style_reference.mjs`, {
  encoding: 'utf8'
})

const pageModified = Promise.all([pageCreated, getStyleScriptLoaded]).then((results) => {
  const page = results[0]
  const getStyleScript = results[1]
  return page.goto(`data:text/html,<script id="style" type="module">
import Vizzu from "${Vizzu}";
${getStyleScript}
</script>`)
})

const selectorLoaded = Promise.all([pageCreated, pageModified]).then((results) => {
  const page = results[0]
  return page.waitForSelector('p')
})

const element = Promise.all([pageCreated, selectorLoaded]).then((results) => {
  const page = results[0]
  return page.$('p')
})

const elementValue = Promise.all([browserLaunched, pageCreated, element]).then((results) => {
  const page = results[1]
  const element = results[2]
  return page.evaluate((el) => el.textContent, element)
})

Promise.all([browserLaunched, elementValue]).then((results) => {
  const browser = results[0]
  browser.close()
})

elementValue.then((elementValue) => {
  let content = ''
  content += `<p id="allbtn-style" class="allbtn-style"><button type="button">+&nbsp;expand all</button></p>`
  content += appendContent(JSON.parse(elementValue), 0)
  content += `<script src="${jsAssetsPath}/style_ref_allbtn.js"></script>`
  content += `<script src="${jsAssetsPath}/style_ref_clickevent.js"></script>`
  console.log(content)
})
