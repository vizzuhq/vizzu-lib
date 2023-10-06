export class Logging {
  meta = { name: 'logging' }

  register(chart) {
    this.chart = chart
  }

  enable(enabled) {
    this.chart._validateModule()
    this.chart._call(this.chart.module._vizzu_setLogging)(enabled)
  }
}
