import { data } from "/test/integration/test_options/style/plot/backgroundColor/static/data.js";
                                                                          

export var chart = chart => chart.animate(
{
    data: data,
    config : {
        channels: {
            x: { attach: [ "Foo"] },
            y: { attach: [ "Bar"] }
        },
        title: null,
        legend: null,
    },
    style: {
        plot: { backgroundColor: window.color }
    }
})
