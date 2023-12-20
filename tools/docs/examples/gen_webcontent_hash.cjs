const fs = require('fs')
const crypto = require('crypto')

async function getWebContentHash() {
	const webcontentPrefixes = [
		'web_content/analytical_operations/',
		'web_content/presets/',
		'web_content/static/'
	]
	const fileContent = fs.readFileSync('./test/e2e/test_cases/test_cases.json', 'utf8')

	const jsonContent = JSON.parse(fileContent)
	let filteredItems = []
	for (const key in jsonContent.test) {
		for (const prefix of webcontentPrefixes) {
			if (key.startsWith(prefix)) {
				filteredItems = filteredItems.concat(jsonContent.test[key].refs)
				break
			}
		}
	}
	const joined = filteredItems.join('')
	const buffer = new Uint16Array(new ArrayBuffer(filteredItems.length * 2))
	for (let i = 0, strLen = filteredItems.length; i < strLen; i++) {
		buffer[i] = joined.charCodeAt(i)
	}
	const hashBuffer = await crypto.subtle.digest('SHA-256', buffer)
	const hashArray = Array.from(new Uint8Array(hashBuffer))
	const hashHex = hashArray.map((b) => b.toString(16).padStart(2, '0')).join('')
	const hash = hashHex.substring(0, 7)
	return hash
}

getWebContentHash().then((hash) => {
	console.log(hash)
})
