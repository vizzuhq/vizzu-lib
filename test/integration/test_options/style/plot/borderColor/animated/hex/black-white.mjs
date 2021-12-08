import Chart from "/test/integration/test_options/style/plot/borderColor/chart.js";
import Hex from "/test/integration/test_options/style/plot/borderColor/animated/hex.js";


const testSteps = [
    Chart.static(Hex.color(new URL(import.meta.url)).color1),
    Chart.animated(Hex.color(new URL(import.meta.url)).color2)
]


export default testSteps