export class CError extends Error {
  constructor(ptr, wasm) {
    const address = parseInt(ptr, 10)
    const type = new wasm.ExceptionInfo(address).get_type()
    const cMessage = wasm._vizzu_errorMessage(address, type)
    const message = wasm.UTF8ToString(cMessage)
    super('error: ' + message)
    this.name = 'CError'
  }
}
