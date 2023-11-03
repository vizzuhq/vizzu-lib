const fs = require('fs')
const path = require('path')

const THRESHOLD = 1.05
const workspacePath = path.dirname(__filename)
const weblibPath = path.join(workspacePath, '../..', 'build/cmake-wasm/weblib')
const sizeFilePath = path.join(weblibPath, 'size.txt')
const sizeDownloadedFilePath = path.join(weblibPath, 'size_downloaded.txt')

fs.readFile(sizeFilePath, 'utf8', (err, sizeData) => {
  if (err) {
    console.error(`Error reading size.txt: ${err}`)
    process.exit(1)
  }

  const sizeLines = sizeData.split('\n').map((line) => line.trim())

  fs.readFile(sizeDownloadedFilePath, 'utf8', (err, downloadedData) => {
    if (err) {
      console.error(`Error reading size_downloaded.txt: ${err}`)
      process.exit(1)
    }

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
          console.error(`Error: File ${file} size exceeds the threshold.`)
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
  })
})
