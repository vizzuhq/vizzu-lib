import { ModuleOptions, CVizzu } from './cvizzu.types'

declare function VizzuModule(options: ModuleOptions): Promise<CVizzu>

export default VizzuModule
