import { CEnv, CObject } from './cenv.js'
import { CPointerClosure } from './objregistry.js'

export class CCanvas extends CObject {
  constructor(env: CEnv, getId: CPointerClosure) {
    super(getId, env)
  }
}
