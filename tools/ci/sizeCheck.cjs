const fs = require('fs')
const path = require('path')

const THRESHOLD = 1.06
const workspacePath = path.dirname(__filename)
const weblibPath = path.join(workspacePath, '../..', 'build/cmake-wasm/weblib')
const sizeFilePath = path.join(weblibPath, 'size.txt')
const sizeDownloadedFilePath = path.join(weblibPath, 'size_downloaded.txt')

const sizeData = fs.readFileSync(sizeFilePath, 'utf8')
const sizeLines = sizeData.split('\n').map((line) => line.trim())

const downloadedData = fs.readFileSync(sizeDownloadedFilePath, 'utf8')
const downloadedLines = downloadedData.split('\n').map((line) => line.trim())

sizeLines.forEach((sizeLine) => {
	if (!sizeLine) return
	const [file, actualSize] = sizeLine.split(' ')

	const downloadedLine = downloadedLines.find((dlLine) => {
		const regex = new RegExp(`^${file}\\s+\\d+`)
		return regex.test(dlLine)
	})

	if (downloadedLine) {
		const downloadedSize = downloadedLine.split(' ')[1]

		if (parseFloat(actualSize) > parseFloat(downloadedSize) * THRESHOLD) {
			console.error(
				`Error: File ${file} size exceeds the threshold. Actual: ${actualSize}, Downloaded: ${downloadedSize}, Threshold: ${THRESHOLD}`
			)
			process.exit(1)
		} else {
			console.log(
				`Success: File ${file} size comparison passed. Actual: ${actualSize}, Downloaded: ${downloadedSize}`
			)
		}
	} else {
		console.warn(
			`Warning: Downloaded size not found for file ${file}. Skipping comparison. Actual: ${actualSize}`
		)
	}
})
