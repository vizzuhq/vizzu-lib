import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(position) {
        return chart => {
            let name = position.pathname.split("/").pop().split(".").shift();
            let position1 = name.split("-").shift().replace("_", "-");
            console.log("plot xAxis label position: " + position1);
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
                                position: position1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(position) {
        return chart => {
            let name = position.pathname.split("/").pop().split(".").shift();
            let position2 = name.split("-").pop().replace("_", "-");
            console.log("plot xAxis label position: " + position2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: { 
                            label: { 
                                position: position2
                            }
                        }
                    }
                }
            })
        };
    }
};
