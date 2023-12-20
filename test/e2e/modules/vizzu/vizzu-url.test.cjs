const VizzuUrl = require('./vizzu-url.cjs')
const path = require('path')
const fs = require('fs')
const fetch = require('node-fetch')

test('if getRemoteBucket()', () => {
	expect(VizzuUrl.getRemoteBucket()).toBe('https://vizzu-lib-main-sha.storage.googleapis.com')
})

test('if getRemoteStableBucket()', () => {
	expect(VizzuUrl.getRemoteStableBucket()).toBe('https://vizzu-lib-main.storage.googleapis.com')
})

test('if getRemoteCdn()', () => {
	expect(VizzuUrl.getRemoteCdn()).toBe('https://cdn.jsdelivr.net/npm/vizzu')
})

test('if getVizzuJs()', () => {
	expect(VizzuUrl.getVizzuJs()).toBe('/vizzu.js')
})

test('if getVizzuMinJs()', () => {
	expect(VizzuUrl.getVizzuMinJs()).toBe('/vizzu.min.js')
})

describe('resolveVizzuUrl()', () => {
	describe('head', () => {
		test('if head', () => {
			return VizzuUrl.resolveVizzuUrl('head').then((url) => {
				expect(url).toBe(
					VizzuUrl.getRemoteStableBucket() + '/lib' + VizzuUrl.getVizzuMinJs()
				)
			})
		})

		test('if head/', () => {
			return VizzuUrl.resolveVizzuUrl('head/').then((url) => {
				expect(url).toBe(
					VizzuUrl.getRemoteStableBucket() + '/lib' + VizzuUrl.getVizzuMinJs()
				)
			})
		})

		test('if head' + VizzuUrl.getVizzuMinJs(), () => {
			return VizzuUrl.resolveVizzuUrl('head' + VizzuUrl.getVizzuMinJs()).then((url) => {
				expect(url).toBe(
					VizzuUrl.getRemoteStableBucket() + '/lib' + VizzuUrl.getVizzuMinJs()
				)
			})
		})

		test('if head' + VizzuUrl.getVizzuJs(), () => {
			return VizzuUrl.resolveVizzuUrl('head' + VizzuUrl.getVizzuJs()).then((url) => {
				expect(url).toBe(VizzuUrl.getRemoteStableBucket() + '/lib' + VizzuUrl.getVizzuJs())
			})
		})
	})

	describe('sha', () => {
		test('if sha is not exist', () => {
			return expect(VizzuUrl.resolveVizzuUrl('0000000')).rejects.toBe(404)
		})

		const shaReady = fetch(VizzuUrl.getRemoteStableBucket() + '/lib/sha.txt').then((shaRaw) => {
			return shaRaw.text()
		})

		test('if sha', () => {
			return shaReady.then((sha) => {
				sha = sha.toString().trim()
				VizzuUrl.resolveVizzuUrl(sha).then((url) => {
					expect(url).toBe(
						VizzuUrl.getRemoteBucket() + '/lib-' + sha + VizzuUrl.getVizzuMinJs()
					)
				})
			})
		})

		test('if sha/', () => {
			return shaReady.then((sha) => {
				sha = sha.toString().trim()
				VizzuUrl.resolveVizzuUrl(sha + '/').then((url) => {
					expect(url).toBe(
						VizzuUrl.getRemoteBucket() + '/lib-' + sha + VizzuUrl.getVizzuMinJs()
					)
				})
			})
		})

		test('if sha' + VizzuUrl.getVizzuMinJs(), () => {
			return shaReady.then((sha) => {
				sha = sha.toString().trim()
				VizzuUrl.resolveVizzuUrl(sha + VizzuUrl.getVizzuMinJs()).then((url) => {
					expect(url).toBe(
						VizzuUrl.getRemoteBucket() + '/lib-' + sha + VizzuUrl.getVizzuMinJs()
					)
				})
			})
		})

		test('if sha' + VizzuUrl.getVizzuJs(), () => {
			return shaReady.then((sha) => {
				sha = sha.toString().trim()
				VizzuUrl.resolveVizzuUrl(sha + VizzuUrl.getVizzuJs()).then((url) => {
					expect(url).toBe(
						VizzuUrl.getRemoteBucket() + '/lib-' + sha + VizzuUrl.getVizzuJs()
					)
				})
			})
		})
	})

	describe('cdn', () => {
		test('if 0.0.0 err is thrown', () => {
			return expect(VizzuUrl.resolveVizzuUrl('0.0.0')).rejects.toBe(404)
		})

		test('if 0.3.0', () => {
			return VizzuUrl.resolveVizzuUrl('0.3.0').then((url) => {
				expect(url).toBe(VizzuUrl.getRemoteCdn() + '@0.3.0/dist' + VizzuUrl.getVizzuMinJs())
			})
		})

		test('if 0.3.0/', () => {
			return VizzuUrl.resolveVizzuUrl('0.3.0/').then((url) => {
				expect(url).toBe(VizzuUrl.getRemoteCdn() + '@0.3.0/dist' + VizzuUrl.getVizzuMinJs())
			})
		})

		test('if 0.3.0' + VizzuUrl.getVizzuMinJs(), () => {
			return VizzuUrl.resolveVizzuUrl('0.3.0' + VizzuUrl.getVizzuMinJs()).then((url) => {
				expect(url).toBe(VizzuUrl.getRemoteCdn() + '@0.3.0/dist' + VizzuUrl.getVizzuMinJs())
			})
		})

		test('if 0.3.0' + VizzuUrl.getVizzuJs(), () => {
			return expect(
				VizzuUrl.resolveVizzuUrl('0.3.0' + VizzuUrl.getVizzuJs())
			).rejects.toThrow('select Vizzu from cdn can be used with vizzu.min.js only')
		})
	})

	describe('local', () => {
		test('if err is thrown (root and dirname are undefined)', () => {
			return expect(VizzuUrl.resolveVizzuUrl('/')).rejects.toThrow('parameter is required')
		})

		test('if err is thrown (root or dirname is undefined)', () => {
			return expect(VizzuUrl.resolveVizzuUrl('/', '/')).rejects.toThrow(
				'parameter is required'
			)
		})

		test('if err is thrown (local does not exist)', () => {
			return expect(VizzuUrl.resolveVizzuUrl('/', '/', '.')).rejects.toThrow(
				"ENOENT: no such file or directory, stat '/vizzu.js'"
			)
		})

		describe('local is exists', () => {
			const root = path.join(__dirname, '../../../..')
			const local = path.join(__dirname, '../../test_report/unit/vizzu')

			beforeAll(() => {
				return new Promise((resolve, reject) => {
					fs.mkdir(local, { recursive: true, force: true }, (err) => {
						if (err) {
							return reject(err)
						}
						const vizzuJsReady = new Promise((resolve, reject) => {
							fs.open(local + VizzuUrl.getVizzuJs(), 'w', (err, file) => {
								if (err) {
									return reject(err)
								}
								return resolve()
							})
						})
						const vizzuMinJsReady = new Promise((resolve, reject) => {
							fs.open(local + VizzuUrl.getVizzuMinJs(), 'w', (err, file) => {
								if (err) {
									return reject(err)
								}
								return resolve()
							})
						})
						Promise.all([vizzuJsReady, vizzuMinJsReady]).then(() => {
							return resolve()
						})
					})
				})
			})

			describe('with relative path', () => {
				const local1 = './test/e2e/test_report/unit/vizzu'
				test('if ' + local1, () => {
					return VizzuUrl.resolveVizzuUrl(local1, root, '.').then((url) => {
						expect(url).toBe(path.resolve(local1 + VizzuUrl.getVizzuJs()))
					})
				})

				const local2 = './test/e2e/test_report/unit/vizzu/'
				test('if ' + local2, () => {
					return VizzuUrl.resolveVizzuUrl(local2, root, '.').then((url) => {
						expect(url).toBe(path.resolve(local2 + 'vizzu.js'))
					})
				})

				const local3 = './test/e2e/test_report/unit/vizzu' + VizzuUrl.getVizzuJs()
				test('if ' + local3, () => {
					return VizzuUrl.resolveVizzuUrl(local3, root, '.').then((url) => {
						expect(url).toBe(path.resolve(local3))
					})
				})

				const local4 = './test/e2e/test_report/unit/vizzu' + VizzuUrl.getVizzuMinJs()
				test('if ' + local4, () => {
					return VizzuUrl.resolveVizzuUrl(local4, root, '.').then((url) => {
						expect(url).toBe(path.resolve(local4))
					})
				})
			})

			describe('with absolute path', () => {
				const local1 = '/test/e2e/test_report/unit/vizzu'
				test('if ' + local1, () => {
					return VizzuUrl.resolveVizzuUrl(local1, root, '.').then((url) => {
						expect(url).toBe(path.resolve(root + local1 + VizzuUrl.getVizzuJs()))
					})
				})

				const local2 = '/test/e2e/test_report/unit/vizzu/'
				test('if ' + local2, () => {
					return VizzuUrl.resolveVizzuUrl(local2, root, '.').then((url) => {
						expect(url).toBe(path.resolve(root + local2 + 'vizzu.js'))
					})
				})

				const local3 = '/test/e2e/test_report/unit/vizzu' + VizzuUrl.getVizzuJs()
				test('if ' + local3, () => {
					return VizzuUrl.resolveVizzuUrl(local3, root, '.').then((url) => {
						expect(url).toBe(path.resolve(root + local3))
					})
				})

				const local4 = '/test/e2e/test_report/unit/vizzu' + VizzuUrl.getVizzuMinJs()
				test('if ' + local4, () => {
					return VizzuUrl.resolveVizzuUrl(local4, root, '.').then((url) => {
						expect(url).toBe(path.resolve(root + local4))
					})
				})
			})

			afterAll(() => {
				return new Promise((resolve, reject) => {
					const rmVizzuJsReady = new Promise((resolve, reject) => {
						fs.rm(
							local + VizzuUrl.getVizzuJs(),
							{ force: true, recursive: true },
							(err) => {
								if (err) {
									return reject(err)
								}
								return resolve()
							}
						)
					})
					const rmVizzuMinJsReady = new Promise((resolve, reject) => {
						fs.rm(
							local + VizzuUrl.getVizzuMinJs(),
							{ force: true, recursive: true },
							(err) => {
								if (err) {
									return reject(err)
								}
								return resolve()
							}
						)
					})
					Promise.all([rmVizzuJsReady, rmVizzuMinJsReady]).then(() => {
						return resolve()
					})
				})
			})
		})
	})
})
