import fs from 'node:fs'

import { CError } from '../../../../../dist/module/cerror.js'
import createVizzuModule from '../../../../../dist/cvizzu.js'

const toCString = (Module, str) => {
	const len = str.length * 4 + 1
	const ptr = Module._malloc(len)
	Module.stringToUTF8(str, ptr, len)
	return ptr
}

describe('module/cerror real wasm', () => {
	let originalFetch

	beforeAll(() => {
		originalFetch = globalThis.fetch
		globalThis.fetch = async (url) => {
			const filePath = new URL(typeof url === 'string' ? url : url.url)
			const bytes = fs.readFileSync(filePath)
			return new Response(bytes, {
				headers: {
					'Content-Type': 'application/wasm'
				}
			})
		}
	})

	afterAll(() => {
		globalThis.fetch = originalFetch
	})

	test('reuses a freed real C++ exception slot after CError wraps it', async () => {
		const Module = await createVizzuModule({
			locateFile: (path) => new URL(`../../../../../dist/${path}`, import.meta.url).href
		})

		const chart = Module._vizzu_createChart()
		const path = toCString(Module, 'invalid')
		try {
			const throwInvalidCommand = () => {
				try {
					Module._anim_control_getValue(chart, path)
				} catch (err) {
					return err
				}
				throw new Error('expected a real C++ exception to be thrown')
			}

			const firstPtr = throwInvalidCommand()
			expect(typeof firstPtr).toBe('number')

			const firstError = new CError(firstPtr, Module)
			expect(firstError.name).toBe('CError')
			expect(firstError.message).toBe('error: invalid animation command')

			const secondPtr = throwInvalidCommand()
			expect(secondPtr).toBe(firstPtr)

			const secondError = new CError(secondPtr, Module)
			expect(secondError.message).toBe('error: invalid animation command')

			const thirdPtr = throwInvalidCommand()
			expect([firstPtr, secondPtr]).toContain(thirdPtr)
		} finally {
			Module._free(path)
			Module._object_free(chart)
		}
	})
})
