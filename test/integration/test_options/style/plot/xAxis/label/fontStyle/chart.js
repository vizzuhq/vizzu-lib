import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(fontStyle) {
        return chart => {
            let name = fontStyle.pathname.split("/").pop().split(".").shift();
            let style1 = name.split("-").shift();
            console.log("plot xAxis label fontStyle: " + style1);
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
                                fontStyle: style1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(fontStyle) {
        return chart => {
            let name = fontStyle.pathname.split("/").pop().split(".").shift();
            let style2 = name.split("-").pop();
            console.log("plot xAxis label fontStyle: " + style2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: { 
                            label: { 
                                fontStyle: style2
                            }
                        }
                    }
                }
            })
        };
    }
};
