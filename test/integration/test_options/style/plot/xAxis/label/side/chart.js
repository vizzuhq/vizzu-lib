import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(side) {
        return chart => {
            let name = side.pathname.split("/").pop().split(".").shift();
            let side1 = name.split("-").shift();
            console.log("plot xAxis label side: " + side1);
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
                            label: { 
                                side: side1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(side) {
        return chart => {
            let name = side.pathname.split("/").pop().split(".").shift();
            let side2 = name.split("-").pop();
            console.log("plot xAxis label side: " + side2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: { 
                            label: { 
                                side: side2
                            }
                        }
                    }
                }
            })
        };
    }
};
