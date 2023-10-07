export class ObjectRegistry {
  constructor(fnFree) {
    this.finalizationRegistry = new FinalizationRegistry((rawCPointer) => {
      fnFree(rawCPointer)
    })
  }

  get(fnGetter) {
    const rawCPointer = fnGetter()
    const object = () => rawCPointer
    this.finalizationRegistry.register(object, rawCPointer)
    return object
  }
}
