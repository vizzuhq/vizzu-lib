import Chart from "/test/integration/test_options/style/plot/borderColor/chart.js";
import Rgba from "/test/integration/test_options/style/plot/borderColor/animated/rgba.js";


const testSteps = [
    Chart.static(Rgba.color(new URL(import.meta.url)).color1),
    Chart.animated(Rgba.color(new URL(import.meta.url)).color2)
]


export default testSteps