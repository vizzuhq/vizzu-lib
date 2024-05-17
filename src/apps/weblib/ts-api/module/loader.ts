import { ModuleOptions } from '../cvizzu.types'

import { Module } from './module.js'
import VizzuModule from '../cvizzu.js'

export interface LoaderOptions {
	/** The URL for the webassembly binary (cvizzu.wasm). */
	wasmUrl?: string
}

class Loader {
	private _options: LoaderOptions
	private _loading: Promise<Module> | null

	constructor() {
		this._options = {}
		this._loading = null
	}

	set options(options: LoaderOptions) {
		this._options = { ...this._options, ...options }
	}

	get options(): LoaderOptions {
		return this._options
	}

	initialize(): Promise<Module> {
		if (!this._loading) this._loading = this._loadModule()
		return this._loading
	}

	private async _loadModule(): Promise<Module> {
		return new Module(await VizzuModule(this._getModuleOptions()))
	}

	private _getModuleOptions(): ModuleOptions {
		const moduleOptions: ModuleOptions = {}
		if (this._options?.wasmUrl) {
			const wasmUrl = this._options.wasmUrl
			moduleOptions.locateFile = (path: string): string => {
				return path.endsWith('.wasm') ? wasmUrl : path
			}
		}
		return moduleOptions
	}
}

export const loader = new Loader()
