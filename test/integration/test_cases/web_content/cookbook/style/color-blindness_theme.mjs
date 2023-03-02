import { data } from '../../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => {
        let style = {
            backgroundColor: "#FFFFFF",
            fontFamily: "Tahoma",
            fontSize: "10px",
            legend: {
                label: {
                    color: "#3E3D3D",
                    fontSize: "1.2em",
                    textAlign: "left",
                },
                title: {
                    color: "#b9b9b9",
                    fontSize: "1.166667em",
                    textAlign: "left",
                },
                marker: {size: "1em", type: "square"},
                paddingLeft: "0.5em",
            },
            plot: {
                marker: {
                    colorGradient: "#003f5c 0, #58508d 0.25, #bc5090 0.5, #ff6361 0.75, #ffa600 1",
                    colorPalette: "#E69F00 "
                        + "#56B4E9 "
                        + "#009E73 "
                        + "#F0E442 "
                        + "#0072B2 "
                        + "#D55E00 "
                        + "#CC79A7 "
                        + "#DEEF14 "
                        + "#AFA037 "
                        + "#9D3B94 "
                        + "#BEB1FD "
                        + "#20756D "
                        + "#E9C66E "
                        + "#BB76D3 "
                        + "#AC1092 ",
                    label: {
                        fontSize: "1.2em",
                        filter: "color(#000000)",
                        fontWeight: "normal",
                    },
                },
                yAxis: {
                    color: "#CCCCCCFF",
                    label: {color: "#999999FF", fontSize: "1em"},
                    title: {color: "#999999FF", fontSize: "1.166667em"},
                },
                xAxis: {
                    label: {color: "#999999FF", fontSize: "1em"},
                    title: {color: "#999999FF", fontSize: "1.166667em"},
                },
            },
            title: {color: "#000000", fontSize: "2.166667em", textAlign: "center"},
            tooltip: {
                arrowSize: "8",
                color: "#191919",
                backgroundColor: "#FFFFFF",
                borderColor: "#D8D8D8FF",
                fontSize: "12px",
            },
            logo: {
                filter: "color(#D8D8D8FF)",
            },
        };

        return chart.animate({
            data: data,
            config: {
                x: 'Year',
                y: ['Value 2 (+)', 'Joy factors'],
                color: 'Joy factors',
                title: 'Dark theme (color palette)'
            },
            style
        })
    },
    chart => chart.animate({
        x: "Year", 
        y: "Country_code", 
        color: "Value 3 (+)", 
        title: "Dark theme (color gradient)"
    })

];

export default testSteps;
