import { data } from "/test/integration/test_options/data.js";


export default class Chart {
    static static(angle) {
        return chart => {
            let nameParts = angle.pathname.split("/").pop().split(".");
            nameParts.pop();
            let name = nameParts.slice().join(".");
            let angle1 = name.split("-").shift();
            console.log("plot xAxis label angle: " + angle1);
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
                                angle: angle1
                            }
                        }
                    }
                }
            });
        }
    }

    static animated(angle) {
        return chart => {
            let nameParts = angle.pathname.split("/").pop().split(".");
            nameParts.pop();
            let name = nameParts.slice().join(".");
            let angle2 = name.split("-").pop();
            console.log("plot xAxis label angle: " + angle2);
            return chart.animate(
            {
                style: {
                    plot: { 
                        xAxis: { 
                            label: { 
                                angle: angle2
                            }
                        }
                    }
                }
            })
        };
    }
};
