window.color = "#" + new URL(import.meta.url).pathname.split("/").pop().split(".").shift();
import { chart } from "/test/integration/test_options/style/plot/backgroundColor/static/chart.js";


const testSteps = [
    chart
]


export default testSteps
