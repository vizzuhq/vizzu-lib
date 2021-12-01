import Chart from "/test/integration/test_options/style/plot/backgroundColor/chart.js";
import Rgba from "/test/integration/test_options/style/plot/backgroundColor/static/rgba.js";


const testSteps = [
    Chart.static(Rgba.color(new URL(import.meta.url)))
]


export default testSteps
