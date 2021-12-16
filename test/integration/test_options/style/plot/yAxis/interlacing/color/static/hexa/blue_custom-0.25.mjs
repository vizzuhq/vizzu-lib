import Chart from "/test/integration/test_options/style/plot/yAxis/interlacing/color/chart.js";
import Hexa from "/test/integration/test_options/style/plot/yAxis/interlacing/color/static/hexa.js";


const testSteps = [
    Chart.static(Hexa.color(new URL(import.meta.url)))
]


export default testSteps
