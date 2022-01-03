import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(orientation) {
        return chart => {
            let name = orientation.pathname.split("/").pop().split(".").shift();
            let orientation1 = name.split("-").shift();
            console.log("plot xAxis label orientation: " + orientation1);
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
                                orientation: orientation1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(orientation) {
        return chart => {
            let name = orientation.pathname.split("/").pop().split(".").shift();
            let orientation2 = name.split("-").pop();
            console.log("plot xAxis label orientation: " + orientation2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: { 
                            label: { 
                                orientation: orientation2
                            }
                        }
                    }
                }
            })
        };
    }
};
