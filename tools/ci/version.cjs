const fs = require('fs')
const path = require('path')

const fullVersion = process.argv[2]
const versionParts = fullVersion.split('.')
const version = versionParts[0] + '.' + versionParts[1]

const readme = path.join(__dirname, '../..', 'README.md')

let content = fs.readFileSync(readme, { encoding: 'utf8', flag: 'r' })

content = content.replaceAll(
	'https://cdn.jsdelivr.net/npm/vizzu@latest/',
	`https://cdn.jsdelivr.net/npm/vizzu@${version}/`
)
content = content.replaceAll(
	'https://lib.vizzuhq.com/latest/',
	`https://lib.vizzuhq.com/${version}/`
)

fs.writeFileSync(readme, content)
