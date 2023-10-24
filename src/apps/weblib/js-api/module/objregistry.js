export class ObjectRegistry {
  constructor(fnFree) {
    this._finalizationRegistry = new FinalizationRegistry((cPointer) => {
      fnFree(cPointer)
    })
  }
  get(fnGetter) {
    const cPointer = fnGetter()
    const object = () => cPointer
    this._finalizationRegistry.register(object, cPointer)
    return object
  }
}
