import { ReadonlyDeep, RequiredDeep } from 'type-fest'

export type Mirrored<T> = ReadonlyDeep<RequiredDeep<T>>
