export class CObject {}

export default class ObjectRegistry {
  constructor(fnFree) {
    this.finalizationRegistry = new FinalizationRegistry((rawCPointer) => {
      fnFree(rawCPointer)
    })
  }

  get(fnGetter, Type) {
    const id = fnGetter()
    const object = new Type()
    object.id = id
    this.finalizationRegistry.register(object, id)
    return object
  }
}
