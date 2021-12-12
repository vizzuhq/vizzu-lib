import { data } from "/test/integration/test_options/data2.js";


export default class Chart {
    static static(width) {
        return chart => {
            let name = width.pathname.split("/").pop().split(".").shift();
            let width1 = name.split("-").shift();
            console.log("plot xAxis ticks lineWidth: " + width1);
            return chart.animate(
            {
                data: data,
                config : {
                    geometry: "circle",
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
                            ticks: {
                                lineWidth: width1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(width) {
        return chart => {
            let name = width.pathname.split("/").pop().split(".").shift();
            let width2 = name.split("-").pop();
            console.log("plot xAxis ticks lineWidth: " + width2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: {
                            ticks: {
                                lineWidth: width2
                            }
                        }
                    }
                }
            })
        };
    }
};
