const path = require("path");


class WorkspacePath {
    static resolvePath(p, root, cwd) {
        if (!p || !root || !cwd) {
            throw new Error("parameter is required");
        }
        if (path.isAbsolute(p)) {
            return path.resolve(path.join(root, p));
        } else {
            return path.resolve(path.join(cwd, p));
        }
    }
}


module.exports = WorkspacePath;
