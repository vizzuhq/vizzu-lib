import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(padding) {
        return chart => {
            let name = padding.pathname.split("/").pop().split(".").shift();
            let padding1 = name.split("-").shift();
            console.log("plot paddingBottom: " + padding1);
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
                        paddingBottom: padding1
                    }
                }
            });
        }
    }

    static animated(padding) {
        return chart => {
            let name = padding.pathname.split("/").pop().split(".").shift();
            let padding2 = name.split("-").pop();
            console.log("plot paddingBottom: " + padding2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        paddingBottom: padding2
                    }
                }
            })
        };
    }
};
