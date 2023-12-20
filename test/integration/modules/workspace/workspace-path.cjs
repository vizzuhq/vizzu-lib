const path = require('path')

class WorkspacePath {
	static resolvePath(p, root, dirname) {
		if (!p || !root || !dirname) {
			throw new Error('parameter is required')
		}
		if (path.isAbsolute(p)) {
			return path.resolve(path.join(root, p))
		} else {
			return path.resolve(path.join(dirname, p))
		}
	}
}

module.exports = WorkspacePath
