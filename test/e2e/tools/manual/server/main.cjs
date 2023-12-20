const WorkspaceHost = require('../../../modules/workspace/workspace-host.cjs')
const TestEnv = require('../../../modules/e2e-test/test-env.cjs')

const LibsHandler = require('./handlers/libs.cjs')
const TestsHandler = require('./handlers/tests.cjs')
const TestCaseHandler = require('./handlers/test-case.cjs')

class ManualServer {
	#workspaceHost
	#workspaceHostReady
	#workspaceHostServerPort

	#testCaseModuleReady

	#configPathList
	#filters

	constructor(configPathList, filters, workspaceHostServerPort) {
		this.#configPathList = configPathList
		this.#filters = filters
		this.#workspaceHostServerPort = workspaceHostServerPort

		this.#testCaseModuleReady = import('../shared/test-case.js')
	}

	run() {
		this.#workspaceHost = new WorkspaceHost(
			TestEnv.getWorkspacePath(),
			this.#workspaceHostServerPort
		)
		this.#workspaceHostReady = this.#workspaceHost.serverPortReady()

		this.#workspaceHostReady.then(() => {
			this.#setRouteGetLibs()
			this.#setRouteGetTests()
			this.#setRouteValidateTestCase()

			console.log(
				`[ W. HOST ] [ http://127.0.0.1:${this.#workspaceHostServerPort}/test/e2e/tools/manual/client ] press CTRL + C to stop`
			)
		})
	}

	#setRouteGetLibs() {
		this.#workspaceHost.setRoute('/getLibs', (req, res) => {
			const libsHandler = new LibsHandler(res, this.#workspaceHostServerPort)
			libsHandler.handle()
		})
	}

	#setRouteGetTests() {
		this.#workspaceHost.setRoute('/getTests', (req, res) => {
			this.#testCaseModuleReady.then((testCaseModule) => {
				const testsHandler = new TestsHandler(
					testCaseModule,
					res,
					this.#configPathList,
					this.#filters
				)
				testsHandler.handle()
			})
		})
	}

	#setRouteValidateTestCase() {
		this.#workspaceHost.setPostRoute('/validateTestCase', (req, res) => {
			this.#testCaseModuleReady.then((testCaseModule) => {
				const testCaseHandler = new TestCaseHandler(testCaseModule, req, res)
				testCaseHandler.handle()
			})
		})
	}
}

module.exports = ManualServer
