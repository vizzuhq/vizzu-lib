import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(color) {
        return chart => {
            console.log("plot xAxis color: " + color);
            return chart.animate(
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
                    plot: { 
                        xAxis: {
                            color: color
                        } 
                    }
                }
            });
        }
    }

    static animated(color) {
        return chart => {
            console.log("plot xAxis color: " + color);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: {
                            color: color
                        } 
                    }
                }
            })
        };
    }
};
