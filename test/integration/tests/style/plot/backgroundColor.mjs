import Chart from "../../chart.mjs";


const testCases = [
    {
        testName: "static_hex_black",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#000000"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_blue",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#0000FF"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_blueCustom",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#1A2B3C"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_blueCustomLowerCase",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#1a2b3c"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_cyan",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#00FFFF"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_lime",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#00FF00"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_magenta",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#FF00FF"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_red",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#FF0000"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_white",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#FFFFFF"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_yellow",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#FFFF00"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_yellowCustom",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#E5D4C3"
                    }
                }
            })
        ]
    },
    {
        testName: "static_hex_yellowCustomLowerCase",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#e5d4c3"
                    }
                }
            })
        ]
    },
    {
        testName: "animated_hex_black-white",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#000000"
                    }
                }
            }),
            Chart.animate({
                style: {
                    plot: { 
                        backgroundColor: "#FFFFFF"
                    }
                }
            })
        ]
    },
    {
        testName: "animated_hex_blueCustom-yellowCustom",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#1A2B3C"
                    }
                }
            }),
            Chart.animate({
                style: {
                    plot: { 
                        backgroundColor: "#E5D4C3"
                    }
                }
            })
        ]
    },
    {
        testName: "animated_hex_white-black",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#FFFFFF"
                    }
                }
            }),
            Chart.animate({
                style: {
                    plot: { 
                        backgroundColor: "#000000"
                    }
                }
            })
        ]
    },
    {
        testName: "animated_hex_yellowCustom-blueCustom",
        testSteps: [
            Chart.animate({
                data: "simpleDataWithOneValue",
                config : "simpleData",
                style: {
                    plot: { 
                        backgroundColor: "#E5D4C3"
                    }
                }
            }),
            Chart.animate({
                style: {
                    plot: { 
                        backgroundColor: "#1A2B3C"
                    }
                }
            })
        ]
    },
]


export default testCases