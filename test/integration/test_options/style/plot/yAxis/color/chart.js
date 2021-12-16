import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(color) {
        return chart => {
            console.log("plot yAxis color: " + color);
            return chart.animate(
            {
                data: data,
                config : {
                    channels: {
                        x: { attach: [ "Bar"] },
                        y: { attach: [ "Foo"] }
                    },
                    title: null,
                    legend: null,
                },
                style: {
                    plot: { 
                        yAxis: {
                            color: color
                        } 
                    }
                }
            });
        }
    }

    static animated(color) {
        return chart => {
            console.log("plot yAxis color: " + color);
            return chart.animate(
            {
                style: {
                    plot: { 
                        yAxis: {
                            color: color
                        } 
                    }
                }
            })
        };
    }
};
