const fs = require('fs')
const fetch = require('node-fetch')

const WorkspacePath = require('../../modules/workspace/workspace-path.cjs')

class VizzuUrl {
	static getRemoteBucket() {
		return 'https://vizzu-lib-main-sha.storage.googleapis.com'
	}

	static getRemoteStableBucket() {
		return 'https://vizzu-lib-main.storage.googleapis.com'
	}

	static getRemoteCdn() {
		return 'https://cdn.jsdelivr.net/npm/vizzu'
	}

	static getVizzuJs() {
		return '/vizzu.js'
	}

	static getVizzuMinJs() {
		return '/vizzu.min.js'
	}

	static resolveVizzuUrl(url, root, dirname) {
		return new Promise((resolve, reject) => {
			url = url.toString()
			const vizzuTypeForced = VizzuUrl.#isVizzuUrlForced(url)
			url = VizzuUrl.#purifyVizzuUrl(url)
			url = VizzuUrl.#completeVizzuUrl(url, vizzuTypeForced)
			if (url.startsWith('https://')) {
				VizzuUrl.#isRemoteVizzuUrlExist(url)
					.then((existingUrl) => {
						return resolve(existingUrl)
					})
					.catch((err) => {
						return reject(err)
					})
			} else {
				VizzuUrl.#isLocalVizzuUrlExist(WorkspacePath.resolvePath(url, root, dirname))
					.then((vizzuPath) => {
						return resolve(vizzuPath)
					})
					.catch((err) => {
						return reject(err)
					})
			}
		})
	}

	static #isVizzuUrlForced(url) {
		if (url.endsWith(VizzuUrl.getVizzuMinJs())) {
			return VizzuUrl.getVizzuMinJs()
		} else if (url.endsWith(VizzuUrl.getVizzuJs())) {
			return VizzuUrl.getVizzuJs()
		}
	}

	static #purifyVizzuUrl(url) {
		if (url.endsWith(VizzuUrl.getVizzuMinJs())) {
			return url.substring(0, url.length - VizzuUrl.getVizzuMinJs().length)
		} else if (url.endsWith(VizzuUrl.getVizzuJs())) {
			return url.substring(0, url.length - VizzuUrl.getVizzuJs().length)
		} else if (url.endsWith('/')) {
			return url.substring(0, url.length - 1)
		} else {
			return url
		}
	}

	static #completeVizzuUrl(url, vizzuTypeForced) {
		if (url.toLowerCase() === 'head') {
			if (!vizzuTypeForced) {
				vizzuTypeForced = VizzuUrl.getVizzuMinJs()
			}
			return VizzuUrl.getRemoteStableBucket() + '/lib' + vizzuTypeForced
		} else if (/^[A-Za-z0-9]+$/.test(url) && url.length === 7) {
			if (!vizzuTypeForced) {
				vizzuTypeForced = VizzuUrl.getVizzuMinJs()
			}
			return VizzuUrl.getRemoteBucket() + '/lib-' + url + vizzuTypeForced
		} else if (/^(\d+\.)?(\d+\.)?(\*|\d+)$/.test(url)) {
			if (vizzuTypeForced === VizzuUrl.getVizzuJs()) {
				throw new Error('select Vizzu from cdn can be used with vizzu.min.js only')
			}
			return VizzuUrl.getRemoteCdn() + '@' + url + '/dist' + VizzuUrl.getVizzuMinJs()
		} else {
			if (!vizzuTypeForced) {
				vizzuTypeForced = VizzuUrl.getVizzuJs()
			}
			return url + vizzuTypeForced
		}
	}

	static #isUrlExist(url) {
		return new Promise((resolve, reject) => {
			fetch(url, {
				method: 'HEAD'
			})
				.then((response) => {
					if (response.status === 200) {
						return resolve(url)
					}
					return reject(response.status)
				})
				.catch((err) => {
					return reject(err)
				})
		})
	}

	static #isRemoteVizzuUrlExist(url) {
		return new Promise((resolve, reject) => {
			VizzuUrl.#isUrlExist(url)
				.then((existingUrl) => {
					return resolve(existingUrl)
				})
				.catch((err) => {
					return reject(err)
				})
		})
	}

	static #isLocalVizzuUrlExist(vizzuPath) {
		return new Promise((resolve, reject) => {
			fs.stat(vizzuPath, (err) => {
				if (err === null) {
					return resolve(vizzuPath)
				} else {
					return reject(err)
				}
			})
		})
	}
}

module.exports = VizzuUrl
