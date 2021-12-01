import Chart from "/test/integration/test_options/style/plot/backgroundColor/chart.js";
import Rgba from "/test/integration/test_options/style/plot/backgroundColor/animated/rgba.js";


const testSteps = [
    Chart.static(Rgba.color(new URL(import.meta.url)).color1),
    Chart.animated(Rgba.color(new URL(import.meta.url)).color2)
]


export default testSteps