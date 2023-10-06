export class CancelError extends Error {
  constructor() {
    super('animation canceled')
    this.name = 'CancelError'
  }
}
