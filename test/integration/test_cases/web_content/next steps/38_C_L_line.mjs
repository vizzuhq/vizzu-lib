import { data } from '../../../test_data/chart_types_eu.mjs';

const testSteps = [
    chart => chart.animate(
        {
            data: Object.assign(data, {
                filter: record =>
                    record.Country == 'Austria' ||
                    record.Country == 'Belgium' ||
                    record.Country == 'Bulgaria' ||
                    record.Country == 'Cyprus' ||
                    record.Country == 'Germany' ||
                    record.Country == 'Denmark' ||
                    record.Country == 'Estonia' ||
                    record.Country == 'Spain' ||
                    record.Country == 'France'
            }),

        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: 'Value 2 (+)' },
                color: { set: 'Country' }
            },
            title: 'Line',
            geometry: 'line'
        },
        style: {
            plot: {
                paddingLeft: '0em',
                yAxis: {
                    label: { paddingRight: '0.8em' },
                    title: { paddingRight: '0.8em' }
                },
                xAxis: {
                    label: { paddingTop: '0.8em' },
                    title: { paddingTop: '2.4em' }
                }
            },
            title: {
                fontSize: '2em'
            }
        }
    },
    {
             delay: 0,
     }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Value 2 (+)', 'Country'] },
            color: { set: 'Country' }
        },
        title: 'Stacked Area / Sum(?), Stack(?)',
        geometry: 'area'
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' }
            }
        }
    }
},
   {
        coordSystem: {
            delay: 0,
            duration: 1,
        },
        geometry: { 
            delay: 0, 
            duration: 0.75, 
//                easing: 'linear' 
        },
        x: {
            delay: 0.5,
            duration: 0.5,
//                easing: 'ease-out'
        }, 
        y: {
            delay: 0,
            duration: 0.75,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
        }
    }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 2 (+)' },
            color: { set: 'Country' }
        },
        title: 'Line / Comparison(?), Components(?)',
        geometry: 'line',
        split: false
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
         delay: 1,
 }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Value 2 (+)', 'Country'] },
            color: { set: 'Country' }
        },
        title: 'Trellis Area / Trellis(?), Components(?), Part-to-whole(?) (sort???)',
        geometry: 'area',
        split: true
    }
},
{
         delay: 1,
 }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 2 (+)' },
            color: { set: 'Country' }
        },
        title: 'Line / Comparison(?)',
        geometry: 'line',
        split: false
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
         delay: 1,
 }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Value 2 (+)', 'Country'] },
            color: { set: 'Country' }
        },
        title: 'Stacked Area / Ratio(?)',
        geometry: 'area',
        align: 'stretch'
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' }
            }
        }
    }
},
   {
    delay: 1,
        coordSystem: {
            delay: 0,
            duration: 1,
        },
        geometry: { 
            delay: 0, 
            duration: 0.75, 
//                easing: 'linear' 
        },
        x: {
            delay: 0.5,
            duration: 0.5,
//                easing: 'ease-out'
        }, 
        y: {
            delay: 0,
            duration: 0.75,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
        }
    }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 2 (+)' },
            color: { set: 'Country' }
        },
        title: 'Line / Comparison(?), Components(?)',
        geometry: 'line',
        align: 'min'
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
         delay: 1,
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Country', 'Value 2 (+)'] },
            size: { set: 'Country' },
            color: { set: 'Country' }
        },
        title: 'Line / Aggregate(?), - Discrete(?)',
        geometry: 'area',
        align: 'min',
        legend: null
    },
    style: {
        plot: {
            paddingLeft: '8em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
         delay: 1,
 }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 2 (+)' },
            size: { set: null },
            color: { set: null }
        },
        title: '2222 Line / Aggregate(?), - Discrete(?)',
        geometry: 'line',
        align: 'min'
    },
    style: {
        plot: {
            paddingLeft: '8em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
     hide: { 
         delay: 0, 
         duration: 1,
         easing: 'ease-in' 
     },
     show: { 
         delay: 0, 
         duration: 1
     },
     geometry: { 
         delay: 0, 
         duration: 0.5,
         easing: 'ease-in' 
     }
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 2 (+)' },
            color: { set: 'Country' },
            size: { set: null },
        },
        title: 'Line / Drill down(?), + Discrete(?)',
        geometry: 'line',
        align: 'min'
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
         delay: 1,
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Year', 'Value 2 (+)'] },
            y: { set: 'Country' },
            color: { set: null }
        },
        title: 'Bar / Sum(?)',
        geometry: 'rectangle',
        align: 'min',
        legend: null
    },
    style: {
        plot: {
            paddingLeft: '8em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
     geometry: { 
         delay: 0.5, 
         duration: 1, 
//                easing: 'linear' 
     },
     x: {
         delay: 0.5,
         duration: 1,
//               easing: 'ease-in'
     }, 
     y: {
         delay: 0,
         duration: 1,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Value 2 (+)'] },
            y: { set: 'Country' },
            color: { set: null },
            label: { set: 'Value 2 (+)' }
        },
        title: 'Bar / Sum(?)',
        geometry: 'rectangle',
        align: 'min',
        legend: null
    },
    style: {
        plot: {
            paddingLeft: '8em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
         delay: 0, 
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Year', 'Value 2 (+)'] },
            y: { set: 'Country' },
            color: { set: null },
            label: { set: null }
        },
        title: 'Bar / Sum(?)',
        geometry: 'rectangle',
        align: 'min',
        legend: null
    },
    style: {
        plot: {
            paddingLeft: '8em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
         delay: 1, 
 }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 2 (+)' },
            color: { set: 'Country' },
            size: { set: null },
        },
        title: 'Line / Drill down timeseries(?), + Discrete(?)',
        geometry: 'line',
        align: 'min',
        legend: 'color'
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
    delay: 0,
     geometry: { 
         delay: 0, 
         duration: 1, 
//                easing: 'linear' 
     },
     x: {
         delay: 0,
         duration: 1,
//               easing: 'ease-in'
     }, 
     y: {
         delay: 0.5,
         duration: 1,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Country', 'Value 2 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Stacked Column / Sum(?), Stack(?)',
        geometry: 'rectangle',
        align: 'min',
        legend: 'color'
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
    delay: 1, 
     geometry: { 
         delay: 0, 
         duration: 1, 
//                easing: 'linear' 
     },
     x: {
         delay: 0,
         duration: 0.5,
//               easing: 'ease-in'
     }, 
     y: {
         delay: 0,
         duration: 0.5,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 2 (+)' },
            color: { set: 'Country' }
        },
        title: 'Line / Comparison(?), + Components(?)',
        geometry: 'line',
        align: 'min'
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
    delay: 0,
     geometry: { 
         delay: 0, 
         duration: 1, 
//                easing: 'linear' 
     },
     x: {
         delay: 0,
         duration: 1,
//               easing: 'ease-in'
     }, 
     y: {
         delay: 0.5,
         duration: 1,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }
 ),



 chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Country', 'Value 2 (+)'] },
            color: { set: 'Country' }
        },
        title: 'Line / Comparison(?), + Components(?)',
        geometry: 'line',
        orientation: 'vertical',
        split: true
    },
    style: {
        plot: {
            paddingLeft: '0em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            }
        }
    }
},
{
    delay: 0,
     geometry: { 
         delay: 0, 
         duration: 1, 
//                easing: 'linear' 
     },
     x: {
         delay: 0,
         duration: 1,
//               easing: 'ease-in'
     }, 
     y: {
         delay: 0,
         duration: 1,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }
 ),


 chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Country' },
            color: { set: null },
            lightness: { set: 'Value 2 (+)' }
        },
        title: 'Heatmap / (?), (?), (sort???)',
        geometry: 'rectangle',
        legend: null,
        split: false
    },
    style: {
        plot: {
            paddingLeft: '8em',
            yAxis: {
                label: { paddingRight: '0.8em' },
                title: { paddingRight: '0.8em' }
            },
            xAxis: {
                label: { paddingTop: '0.8em' },
                title: { paddingTop: '2.4em' }
            },
            marker: {
                rectangleSpacing: 0
            }    
        }
    }
},
{
    delay: 0,
    style: { 
        delay: 0, 
        duration: 0.25 
    },
     geometry: { 
         delay: 0, 
         duration: 0.5, 
//                easing: 'linear' 
     },
     x: {
         delay: 0,
         duration: 0.5,
//               easing: 'ease-in'
     }, 
     y: {
         delay: 0,
         duration: 0.5,
//                easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }
)

];

export default testSteps;