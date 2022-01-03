import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(textAlign) {
        return chart => {
            let name = textAlign.pathname.split("/").pop().split(".").shift();
            let textAlign1 = name.split("-").shift();
            console.log("plot xAxis label textAlign: " + textAlign1);
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
                                textAlign: textAlign1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(textAlign) {
        return chart => {
            let name = textAlign.pathname.split("/").pop().split(".").shift();
            let textAlign2 = name.split("-").pop();
            console.log("plot xAxis label textAlign: " + textAlign2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: { 
                            label: { 
                                textAlign: textAlign2
                            }
                        }
                    }
                }
            })
        };
    }
};
