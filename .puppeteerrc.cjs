const { join } = require('path')

module.exports = {
	cacheDirectory: join(__dirname, 'node_modules', '.cache', 'puppeteer')
}
