import Chart from "/test/integration/test_options/style/plot/borderColor/chart.js";
import Rgb from "/test/integration/test_options/style/plot/borderColor/static/rgb.js";


const testSteps = [
    Chart.static(Rgb.color(new URL(import.meta.url)))
]


export default testSteps
