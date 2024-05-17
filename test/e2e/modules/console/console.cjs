const path = require('path')
const fs = require('fs')
const cnsl = require('console')
const strip = require('strip-color')
const colors = require('colors')
colors.setTheme({
	warn: 'yellow',
	error: 'red',
	success: 'green'
})

class Console {
	#timeStamp
	#logFile
	#fileConsoleReady

	constructor(logPrefix = undefined, logPath = undefined) {
		this.#setTimestamp(new Date())
		this.#setLogFile(logPrefix, logPath)
	}

	log(msg) {
		return new Promise((resolve, reject) => {
			if (!msg) {
				// eslint-disable-next-line prefer-promise-reject-errors
				return reject('parameter is required')
			}
			console.log(msg)
			if (!this.#logFile) {
				return resolve()
			} else {
				if (!this.#fileConsoleReady) {
					this.#fileConsoleReady = this.#setFileConsole()
				}
				this.#fileConsoleReady
					.then((fileConsole) => {
						fileConsole.log(strip(msg))
						return resolve()
					})
					.catch((err) => {
						return reject(err)
					})
			}
		})
	}

	getTimeStamp() {
		return this.#timeStamp
	}

	getLogFile() {
		return this.#logFile
	}

	#setTimestamp(date) {
		this.#timeStamp =
			date.getFullYear() +
			(date.getMonth() + 1).toString().padStart(2, 0) +
			date.getDate().toString().padStart(2, 0) +
			'_' +
			date.getHours().toString().padStart(2, 0) +
			date.getMinutes().toString().padStart(2, 0) +
			date.getSeconds().toString().padStart(2, 0)
	}

	#setLogFile(logPrefix, logPath) {
		if (logPrefix && logPath) {
			this.#logFile = path.join(logPath, logPrefix + '_' + this.#timeStamp + '.log')
		} else if (logPrefix && !logPath) {
			this.#logFile = path.join(process.cwd(), logPrefix + '_' + this.#timeStamp + '.log')
		}
	}

	#setFileConsole() {
		return new Promise((resolve, reject) => {
			fs.mkdir(path.dirname(this.#logFile), { recursive: true }, (err) => {
				if (err) {
					return reject(err)
				}
				const out = fs.createWriteStream(this.#logFile)
				return resolve(new cnsl.Console(out, out))
			})
		})
	}
}

module.exports = Console
