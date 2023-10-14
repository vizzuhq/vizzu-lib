import { ModuleOptions } from '../cvizzu.types'

import { Module } from './module.js'
import VizzuModule from '../cvizzu.js'

interface LoaderOptions {
  wasmUrl?: string
}

class Loader {
  _options: LoaderOptions
  _loading: Promise<Module> | null

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

  async _loadModule(): Promise<Module> {
    return new Module(await VizzuModule(this._getModuleOptions()))
  }

  _getModuleOptions(): ModuleOptions {
    const moduleOptions: ModuleOptions = {}
    if (this._options?.wasmUrl) {
      const wasmUrl = this._options.wasmUrl
      moduleOptions.locateFile = (path: string) => {
        return path.endsWith('.wasm') ? wasmUrl : path
      }
    }
    return moduleOptions
  }
}

export const loader = new Loader()
