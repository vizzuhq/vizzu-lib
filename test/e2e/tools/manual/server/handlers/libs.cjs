const fetch = require('node-fetch')

const VizzuUrl = require('../../../../modules/vizzu/vizzu-url.cjs')
const VizzuCloudFunctions = require('../../../../modules/vizzu/vizzu-cloudfunctions.cjs')
const VizzuVersion = require('../../../../modules/vizzu/vizzu-version.cjs')

class LibsHandler {
	constructor(res, workspaceHostServerPort) {
		this.res = res
		this.workspaceHostServerPort = workspaceHostServerPort
	}

	handle() {
		const localReady = this.#getLocal()
		const headReady = this.#getHead()
		const shaLibsReady = this.#getSHALibs()
		const cdnLibsReady = this.#getCDNLibs()

		Promise.all([localReady, headReady, shaLibsReady, cdnLibsReady])
			.then((versions) => {
				this.res.send(Object.assign({}, ...versions))
			})
			.catch((err) => {
				console.error(err)
				this.res.status(500).send('internal server error')
			})
	}

	#getLocal() {
		const localUrl = `http://127.0.0.1:${this.workspaceHostServerPort}/dist/vizzu.js`
		return VizzuVersion.checkUrlAvailability(localUrl)
			.then(() => ({ localhost: localUrl }))
			.catch((err) => {
				console.error(err)
				console.error('failed to fetch localhost')
				return {}
			})
	}

	#getHead() {
		const headUrl = VizzuUrl.getRemoteStableBucket() + '/lib/vizzu.js'
		return VizzuVersion.checkUrlAvailability(headUrl)
			.then(() => ({ HEAD: headUrl }))
			.catch((err) => {
				console.error(err)
				console.error('failed to fetch head')
				return {}
			})
	}

	#getSHALibs() {
		return this.#fetchSHALibs()
			.then((vizzuList) => {
				const versions = {}
				vizzuList.reverse().forEach((vizzu) => {
					versions[
						`${vizzu.time.substring(0, 10)} ${vizzu.time.substring(11, 16)} ${
							vizzu.sha
						}`
					] = VizzuUrl.getRemoteBucket() + '/' + vizzu.sha
				})
				return versions
			})
			.catch((err) => {
				console.error(err)
				console.error('failed to fetch sha lib list')
				return {}
			})
	}

	#getCDNLibs() {
		return VizzuVersion.getPublicBetaList()
			.then((cdnList) => {
				const versions = {}
				cdnList.forEach((version) => {
					versions[version.num] = version.url
				})
				return versions
			})
			.catch((err) => {
				console.error(err)
				console.error('failed to fetch public beta list')
				return {}
			})
	}

	#fetchSHALibs() {
		return fetch(VizzuCloudFunctions.getRemoteCloudFunctions() + '/getVizzuList')
			.then((vizzuListUrl) => vizzuListUrl.json())
			.catch((err) => {
				console.error(err)
				return []
			})
	}
}

module.exports = LibsHandler
