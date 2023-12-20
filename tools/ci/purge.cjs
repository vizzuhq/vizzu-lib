const axios = require('axios')

function getPurgeData(version) {
	const versionParts = version.split('.')
	const versions = [
		'latest',
		`${versionParts[0]}`,
		`${versionParts[0]}.${versionParts[1]}`,
		version
	]
	const purgeData = {
		path: []
	}
	for (const item of versions) {
		const pathTemplate = `/npm/vizzu@${item}`
		purgeData.path.push(`${pathTemplate}/dist/vizzu.min.js`)
		purgeData.path.push(`${pathTemplate}/dist/cvizzu.wasm`)
		purgeData.path.push(`${pathTemplate}/dist/vizzu.d.ts`)
	}
	return purgeData
}

async function purgeCDN(purgeData) {
	const purgeResponse = await axios.post('https://purge.jsdelivr.net/', purgeData, {
		headers: {
			'Content-Type': 'application/json'
		}
	})
	if (purgeResponse.status === 202) {
		const purgeResult = purgeResponse.data
		console.log('Purge ID:', purgeResult.id)
		const statusData = await checkPurgeIsFinished(purgeResult.id, 5000, 100)
		if (checkPurgeIsSucceeded(statusData)) {
			console.log('Purge succeeded')
		} else {
			console.log(statusData)
			throw new Error('Purge failed')
		}
	} else {
		throw new Error(`Purge request failed ${purgeResponse.status}`)
	}
}

async function checkPurgeIsFinished(purgeId, timeout, checkInterval) {
	const startTime = Date.now()
	while (Date.now() - startTime < timeout) {
		const statusResponse = await axios.get(`https://purge.jsdelivr.net/status/${purgeId}`)
		if (statusResponse.status === 200) {
			if (statusResponse.data.status === 'finished') {
				console.log('Purge Status:', statusResponse.data.status)
				return statusResponse.data
			}
		} else {
			throw new Error(`Failed to fetch purge status: ${statusResponse.status}`)
		}
		await new Promise((resolve) => setTimeout(resolve, checkInterval))
	}
	throw new Error('Failed to fetch purge status: timeout')
}

function checkPurgeIsSucceeded(data) {
	for (const path in data.paths) {
		const pathData = data.paths[path]
		if (pathData.throttled !== false) {
			return false
		}
		for (const provider in pathData.providers) {
			const providerData = pathData.providers[provider]
			for (const key in providerData) {
				if (providerData[key] !== true) {
					return false
				}
			}
		}
	}
	return true
}

const version = process.argv[2]
const purgeData = getPurgeData(version)
purgeCDN(purgeData)
