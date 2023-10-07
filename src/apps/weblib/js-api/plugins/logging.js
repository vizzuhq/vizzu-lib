export class Logging {
  meta = { name: 'logging' }

  register(chart) {
    this.chart = chart
  }

  enable(enabled) {
    this.chart._validateModule()
    this.chart._module.setLogging(enabled)
  }
}
