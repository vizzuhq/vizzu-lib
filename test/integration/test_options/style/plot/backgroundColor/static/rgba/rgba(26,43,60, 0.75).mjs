window.color = new URL(import.meta.url).pathname.split("/").pop().split(".mjs").shift().replaceAll("%20", " ");
import { chart } from "/test/integration/test_options/style/plot/backgroundColor/static/chart.js";


const testSteps = [
    chart
]


export default testSteps
