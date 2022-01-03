import Chart from "/test/integration/test_options/style/plot/xAxis/label/angle/chart.js";


const testSteps = [
    Chart.static(new URL(import.meta.url)),
    Chart.animated(new URL(import.meta.url))
]


export default testSteps
