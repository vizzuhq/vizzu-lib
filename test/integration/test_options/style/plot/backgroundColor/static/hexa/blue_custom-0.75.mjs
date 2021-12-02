import Chart from "/test/integration/test_options/style/plot/backgroundColor/chart.js";
import Hexa from "/test/integration/test_options/style/plot/backgroundColor/static/hexa.js";


const testSteps = [
    Chart.static(Hexa.color(new URL(import.meta.url)))
]


export default testSteps
