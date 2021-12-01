import Chart from "/test/integration/test_options/style/plot/backgroundColor/chart.js";
import Rgb from "/test/integration/test_options/style/plot/backgroundColor/static/rgb.js";


const testSteps = [
    Chart.static(Rgb.color(new URL(import.meta.url)))
]


export default testSteps
