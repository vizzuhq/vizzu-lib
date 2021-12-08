import Chart from "/test/integration/test_options/style/plot/borderColor/chart.js";
import Hex from "/test/integration/test_options/style/plot/borderColor/static/hex.js";


const testSteps = [
    Chart.static(Hex.color(new URL(import.meta.url)))
]


export default testSteps
