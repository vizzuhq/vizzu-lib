// export GOOGLE_APPLICATION_CREDENTIALS="~/.gcp/vizzu-ci.json"

exports.getVizzuList = (req, res) => {
	const { Storage } = require('@google-cloud/storage')
	const storage = new Storage()
	const bucketName = 'vizzu-lib-main-sha'
	const bucket = storage.bucket(bucketName)
	bucket.getFiles().then((files) => {
		const ans = []
		files[0].sort((a, b) => {
			if (a.metadata.updated > b.metadata.updated) {
				return 1
			}
			if (a.metadata.updated < b.metadata.updated) {
				return -1
			}
			return 0
		})
		files[0].forEach((file) => {
			const fileEnd = '/vizzu.js'
			if (file.name.endsWith(fileEnd)) {
				const tmp = {}
				tmp.sha = file.name.slice(0, -fileEnd.length)
				tmp.time = file.metadata.updated
				ans.push(tmp)
			}
		})
		const message = req.query.message || req.body.message || JSON.stringify(ans)
		res.status(200).send(message)
	})
}
