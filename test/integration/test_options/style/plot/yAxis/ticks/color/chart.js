import { data } from "/test/integration/test_options/data2.js";


export default class Chart {
    static static(color) {
        return chart => {
            console.log("plot yAxis ticks color: " + color);
            return chart.animate(
            {
                data: data,
                config : {
                    geometry: 'circle',
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
                                length: 5,
                                lineWidth: 5,
                                color: color
                            }
                        } 
                    }
                }
            });
        }
    }

    static animated(color) {
        return chart => {
            console.log("plot yAxis ticks color: " + color);
            return chart.animate(
            {
                style: {
                    plot: { 
                        yAxis: {
                            ticks: {
                                length: 5,
                                lineWidth: 5,
                                color: color
                            }
                        } 
                    }
                }
            })
        };
    }
};
