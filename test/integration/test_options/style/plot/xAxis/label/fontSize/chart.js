import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(fontSize, type) {
        return chart => {
            let name = fontSize.pathname.split("/").pop().split(".").shift();
            let fontSize1 = name.split("-").shift();
            if (type) {
                fontSize1 = fontSize1.toString() + type;
            }
            console.log("plot xAxis label fontSize: " + fontSize1);
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
                                fontSize: fontSize1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(fontSize, type) {
        return chart => {
            let name = fontSize.pathname.split("/").pop().split(".").shift();
            let fontSize2 = name.split("-").pop();
            if (type) {
                fontSize2 = fontSize2.toString() + type;
            }
            console.log("plot xAxis label fontSize: " + fontSize2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: {
                            label: {
                                fontSize: fontSize2
                            }
                        }
                    }
                }
            })
        };
    }
};
