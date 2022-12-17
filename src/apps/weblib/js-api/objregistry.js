export default class ObjectRegistry {
  constructor(fnFree) {
    this.finalizationRegistry = new FinalizationRegistry((rawCPointer) => {
      fnFree(rawCPointer);
    });
  }

  get(fnGetter) {
    let id = fnGetter();
    let object = { id };
    this.finalizationRegistry.register(object, id);
    return object;
  }
}
