import { CEnv, CObject } from './cenv'

export class CCanvas extends CObject {
	constructor(env: CEnv) {
		const getId = env._getStatic(env._wasm._vizzu_createCanvas)
		super(getId, env)
	}
}
