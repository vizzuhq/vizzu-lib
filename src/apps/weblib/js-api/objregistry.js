export default class ObjectRegistry {
  constructor(fnFree) {
    this.finalizationRegistry = new FinalizationRegistry((rawCPointer) => {
      fnFree(rawCPointer)
    })
  }

  get(fnGetter, Type) {
    let id = fnGetter()
    let object = new Type()
    object.id = id
    this.finalizationRegistry.register(object, id)
    return object
  }
}
