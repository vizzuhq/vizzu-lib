const WorkspacePath = require('./workspace-path.cjs')

describe('WorkspacePath.resolvePath() without sufficient params', () => {
	test('if err is thrown (0 param)', () => {
		expect(() => {
			WorkspacePath.resolvePath()
		}).toThrow('parameter is required')
	})

	test('if err is thrown (1 param)', () => {
		const param1 = 'param1'
		expect(() => {
			WorkspacePath.resolvePath(param1)
		}).toThrow('parameter is required')
	})

	test('if err is thrown (2 params)', () => {
		const param1 = 'param1'
		const param2 = 'param2'
		expect(() => {
			WorkspacePath.resolvePath(param1, param2)
		}).toThrow('parameter is required')
	})
})

describe('WorkspacePath.resolvePath(p, root, dirname)', () => {
	test('if is absolute path resolved', () => {
		const p = '/testDir/testFile.js'
		const root = '/'
		const dirname = '/testDirname'
		expect(WorkspacePath.resolvePath(p, root, dirname)).toBe('/testDir/testFile.js')
	})

	test('if is relative path resolved', () => {
		const p = './testDir/testFile.js'
		const root = '/'
		const dirname = '/testDirname'
		expect(WorkspacePath.resolvePath(p, root, dirname)).toBe('/testDirname/testDir/testFile.js')
	})
})
