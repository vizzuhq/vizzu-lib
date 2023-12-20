const Console = require('./console.cjs')
const path = require('path')
const fs = require('fs')

describe('new Console()', () => {
	test('if logFile is undefined', () => {
		const cnsl = new Console()
		const logFile = cnsl.getLogFile()
		expect(logFile).toBeUndefined()
	})

	test('if timeStamp is valid', () => {
		const startDate = new Date()
		const cnsl = new Console()
		const timeStamp = cnsl.getTimeStamp()
		expect(timeStamp.length).toBe(15)
		const separator = timeStamp.substring(8, 9)
		expect(separator).toBe('_')
		const year = timeStamp.substring(0, 4)
		const month = timeStamp.substring(4, 6) - 1
		const day = timeStamp.substring(6, 8)
		const hour = timeStamp.substring(9, 11)
		const min = timeStamp.substring(11, 13)
		const sec = timeStamp.substring(13, 15)
		const timeStampDateMin = new Date(year, month, day, hour, min, sec, 0)
		const timeStampDateMax = new Date(year, month, day, hour, min, sec, 999)
		const endDate = new Date()
		expect(timeStampDateMin).toBeBeforeOrEqualTo(endDate)
		expect(timeStampDateMax).toBeAfterOrEqualTo(startDate)
	})
})

describe('new Console(logPrefix)', () => {
	test("if logFile's dirname is __dirname", () => {
		const cnsl = new Console(logPrefix)
		const logFile = cnsl.getLogFile()
		expect(path.dirname(logFile)).toBe(process.cwd())
	})

	const logPrefix = 'logPrefix'
	const cwdSaved = process.cwd()
	process.chdir('../')
	const cwd = process.cwd()
	const cnsl = new Console(logPrefix)
	const logFile = cnsl.getLogFile()
	process.chdir(cwdSaved)
	test("if logFile's dirname is cwd", () => {
		expect(path.dirname(logFile)).toBe(cwd)
	})

	test("if logFile's basename starts with logPrefix", () => {
		expect(path.basename(logFile)).toStartWith('logPrefix')
	})
})

describe('new Console(logPrefix, logPath)', () => {
	const logPrefix = 'logPrefix'
	const logPath = './logPath'
	const cnsl = new Console(logPrefix, logPath)
	const logFile = cnsl.getLogFile()
	const timeStamp = cnsl.getTimeStamp()
	test('if logFile is logPath/logPrefix_timeStamp.log', () => {
		expect(logFile).toBe(path.join(logPath, logPrefix + '_' + timeStamp + '.log'))
	})
})

describe('new Console().cnsl.log()', () => {
	test('if cnsl.log() does not log', () => {
		const cnsl = new Console()
		return expect(cnsl.log()).rejects.toBe('parameter is required')
	})

	test('if cnsl.log("Hello World") logs "Hello World"', () => {
		const msg = 'Hello World'
		const somethingSpy = jest.spyOn(console, 'log').mockImplementation(() => {})
		const cnsl = new Console()
		return cnsl.log(msg).then(() => {
			expect(somethingSpy).toBeCalledWith(msg)
		})
	})
})

describe('new Console(logPrefix, logPath|undefined).cnsl.log()', () => {
	let logFile

	afterEach(() => {
		fs.rmSync(logFile, { force: true, recursive: true })
	})

	test('if cnsl.log("Hello World") logs "Hello World" (logPath is undefined)', () => {
		const logPrefix = 'logPrefix'
		const cnsl = new Console(logPrefix)
		logFile = cnsl.getLogFile()
		const msg = 'Hello World'
		const somethingSpy = jest.spyOn(console, 'log').mockImplementation(() => {})

		return cnsl.log(msg).then(() => {
			expect(somethingSpy).toBeCalledWith(msg)
		})
	})

	test('if cnsl.log("Hello World") logs "Hello World" (logPath)', () => {
		const logPrefix = 'logPrefix'
		const logPath = path.join(__dirname, '../..', 'test_report/unit/console')
		const cnsl = new Console(logPrefix, logPath)
		logFile = cnsl.getLogFile()
		const msg = 'Hello World'
		const somethingSpy = jest.spyOn(console, 'log').mockImplementation(() => {})

		return cnsl.log(msg).then(() => {
			expect(somethingSpy).toBeCalledWith(msg)
		})
	})

	test('if 2xcnsl.log() logs 2x (logPath)', async () => {
		const logPrefix = 'logPrefix'
		const logPath = path.join(__dirname, '../..', 'test_report/unit/console')
		const cnsl = new Console(logPrefix, logPath)
		logFile = cnsl.getLogFile()
		const msg1 = 'warn'
		const msg2 = 'error'
		const msg3 = 'success'
		const msg4 = 'Hello World'
		const somethingSpy = jest.spyOn(console, 'log').mockImplementation(() => {})

		const logReady = []
		logReady.push(cnsl.log(msg1.warn))
		logReady.push(cnsl.log(msg2.error))
		logReady.push(cnsl.log(msg3.success))
		logReady.push(cnsl.log(msg4))
		await Promise.all(logReady)

		expect(somethingSpy).toBeCalledTimes(4)
		expect(somethingSpy).toBeCalledWith(msg1.warn)
		expect(somethingSpy).toBeCalledWith(msg2.error)
		expect(somethingSpy).toBeCalledWith(msg3.success)
		expect(somethingSpy).toBeCalledWith(msg4)

		const checkFileContent = () => {
			return fs.promises
				.access(cnsl.getLogFile())
				.then(() => fs.promises.readFile(cnsl.getLogFile(), 'utf8'))
				.then((data) => {
					return data
				})
				.catch((err) => {
					if (err.code === 'ENOENT') {
						return ''
					} else {
						throw err
					}
				})
		}

		const checkFileWithTimeout = async (expectedContent, timeoutMs, intervalMs) => {
			const startTime = Date.now()
			while (Date.now() - startTime < timeoutMs) {
				const fileContent = await checkFileContent()
				if (fileContent === expectedContent) {
					return
				}
				await new Promise((resolve) => setTimeout(resolve, intervalMs))
			}
			throw new Error(`File content did not match within ${timeoutMs}ms`)
		}

		const expectedContent = msg1 + '\n' + msg2 + '\n' + msg3 + '\n' + msg4 + '\n'
		await checkFileWithTimeout(expectedContent, 2000, 100)
	})
})
