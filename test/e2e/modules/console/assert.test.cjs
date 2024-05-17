const assert = require('./assert.cjs')

describe('assert()', () => {
	test('if passed', () => {
		expect(() => {
			assert(true)
		}).not.toThrow()
	})

	test('if failed without msg', () => {
		expect(() => {
			assert(false)
		}).toThrow('Assert failed: ')
	})

	test('if failed with msg', () => {
		expect(() => {
			assert(false, 'err msg')
		}).toThrow('Assert failed: err msg')
	})
})
