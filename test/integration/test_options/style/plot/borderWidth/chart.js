import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(width) {
        return chart => {
            let name = width.pathname.split("/").pop().split(".").shift();
            let width1 = name.split("-").shift();
            console.log("plot borderWidth: " + width1);
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
                        borderWidth: width1,
                        borderColor: "#000000"
                    }
                }
            });
        }
    }

    static animated(width) {
        return chart => {
            let name = width.pathname.split("/").pop().split(".").shift();
            let width2 = name.split("-").pop();
            console.log("plot borderWidth: " + width2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        borderWidth: width2,
                        borderColor: "#000000" 
                    }
                }
            })
        };
    }
};
