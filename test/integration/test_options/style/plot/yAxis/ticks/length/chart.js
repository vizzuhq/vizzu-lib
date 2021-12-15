import { data } from "/test/integration/test_options/data2.js";


export default class Chart {
    static static(width, type) {
        return chart => {
            let name = width.pathname.split("/").pop().split(".").shift();
            let length1 = name.split("-").shift();
            if (type) {
                length1 = length1.toString() + type;
            }
            console.log("plot yAxis ticks length: " + length1);
            return chart.animate(
            {
                data: data,
                config : {
                    geometry: "circle",
                    orientation: 'vertical',
                    channels: {
                        x: { attach: [ "Foo"] },
                        y: { attach: [ "Bar"] }
                    },
                    title: null,
                    legend: null,
                },
                style: {
                    plot: { 
                        yAxis: {
                            ticks: {
                                length: length1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(width, type) {
        return chart => {
            let name = width.pathname.split("/").pop().split(".").shift();
            let length2 = name.split("-").pop();
            if (type) {
                length2 = length2.toString() + type;
            }
            console.log("plot yAxis ticks length: " + length2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        yAxis: {
                            ticks: {
                                length: length2
                            }
                        }
                    }
                }
            })
        };
    }
};
