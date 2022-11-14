import { data } from '../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: Object.assign(data, {
            filter: record =>
                record.Country == 'Austria' ||
                record.Country == 'Belgium' ||
                record.Country == 'Bulgaria' ||
                record.Country == 'Cyprus' ||
                record.Country == 'Czechia' ||
                record.Country == 'Denmark' ||
                record.Country == 'Estonia' ||
                record.Country == 'Greece' ||
                record.Country == 'Germany' ||
                record.Country == 'Spain' ||
                record.Country == 'Finland' ||
                record.Country == 'France' ||
                record.Country == 'Croatia' ||
                record.Country == 'Hungary'
        }),
        config: {
            channels: {
                x: { set: 'Country' },
                y: { set: 'Value 2 (+)' },
//                color: { set: 'Country' },
                label: { set: 'Value 2 (+)' }
            },
            title: 'Column',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        },
        style: {
            plot: {
                xAxis: {
                    label: {
                        angle: '2.5'
                    }
                },
                marker: {
                    label: {
                        fontSize: 11
                    }
                }
        }
    }
    }),
    chart => chart.animate({
        config: {
            channels: {
               x: { set: 'Country' },
                y: { set: ['Value 2 (+)', 'Joy factors'] },
                color: { set: 'Joy factors' },
                label: { set: 'Value 2 (+)' }
            },
            title: 'Stacked Column / Drill down(?), + Discrete(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: 'color'
        },
        style: {
            plot: {
                marker: {
                    label: {
                        fontSize: 9
                    }
                }
    }
}
}),
chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Country' },
                y: { set: 'Value 2 (+)' },
                color: { set: null },
                label: { set: 'Value 2 (+)' }
            },
            title: 'Column / Aggregate(?), - Discrete(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        },
        style: {
            plot: {
                xAxis: {
                    label: {
                        angle: '2.5'
                    }
                }
        }
    }
    }),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: null },
                label: { set: null }
            },
            title: 'Stacked Column / Sum(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: 'Value 2 (+)' },
                color: { set: null },
                label: { set: 'Value 2 (+)' }
            },
            title: 'Stacked Column / Total(?), Aggregate(?), - Discrete(?), Total(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        }
    }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Country', 'Value 2 (+)'] },
                color: { set: null },
                label: { set: null }
            },
            title: 'Stacked Column / Drill down(?), + Discrete(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        }
    }
    ),

    chart => chart.animate({
            config: {
            channels: {
                x: { set: 'Country' },
                y: { set: 'Value 2 (+)' },
                color: { set: null },
                label: { set: 'Value 2 (+)' }
            },
            title: 'Column - Group(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            legend: null
        },
        style: {
            plot: {
                xAxis: {
                    label: {
                        angle: '2.5'
                    }
            }
        }
    }
    }),

    chart => chart.animate({
        config: {
        channels: {
            x: { set: 'Value 5 (+/-)' },
            y: { set: 'Value 2 (+)' },
            color: { set: 'Country' },
            label: { set: null }
        },
        title: 'Bubble plot / + Continuous(?)',
        geometry: 'circle',
        orientation: 'horizontal',
        legend: 'color'
    },
    style: {
        plot: {
            xAxis: {
                label: {
                    angle: '3.14'
                }
        }
    }
}
},
{
    style: {
        delay: 0.5,
        duration: 1.5,
    },
     legend: {
         delay: 0.5,
         duration: 1.5,
     },
     x: {
         delay: 1,
         duration: 1,
//                easing: 'ease-out'
     }, 
     y: {
         delay: 0,
         duration: 1,
//             easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }
 ),

 chart => chart.animate({
         config: {
         channels: {
             x: { set: 'Country' },
             y: { set: 'Value 2 (+)' },
             color: { set: null },
             label: { set: 'Value 2 (+)' }
         },
         title: 'Column / - Continuous(?)',
         geometry: 'rectangle',
         orientation: 'horizontal',
         legend: null
     },
     style: {
         plot: {
             xAxis: {
                 label: {
                     angle: '2.5'
                 }
         }
     }
 }
 },
{
    style: {
        delay: 0,
        duration: 1,
    },
     legend: {
         delay: 0,
         duration: 1,
     },
     x: {
         delay: 0,
         duration: 1,
//                easing: 'ease-out'
     }, 
     y: {
         delay: 1,
         duration: 1,
//             easing: 'cubic-bezier(.39,0,.35,.99)'
     }
 }
 ),
 chart => chart.animate({
    config: {
    channels: {
        x: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' } },
        y: { set: null },
        color: { set: null },
        label: { set: null }
    },
    title: 'Bar / Sum(?)',
    geometry: 'rectangle',
    orientation: 'vertical',
    legend: null
},
style: {
    plot: {
       marker: {
            label: {
                fontSize: 9
        }
    },
    xAxis: {
        label: {
            angle: 3.14
        }
}
}
}
},
{
style: {
   delay: 1,
   duration: 1,
},
legend: {
    delay: 1,
    duration: 1,
},
x: {
    delay: 1,
    duration: 1,
//                easing: 'ease-out'
}, 
y: {
    delay: 0,
    duration: 1,
//             easing: 'cubic-bezier(.39,0,.35,.99)'
}
}
),


chart => chart.animate({
    config: {
    channels: {
        x: { set: ['Value 2 (+)'], range: { min: '0%', max: '110%' } },
        y: { set: null },
        color: { set: null },
        label: { set: 'Value 2 (+)' }
    },
    title: 'Bar / Total(?)',
    geometry: 'rectangle',
    orientation: 'vertical',
    legend: null
},
style: {
    plot: {
       marker: {
            label: {
                fontSize: 11
        }
    }
}
}
},
{
style: {
   delay: 1,
   duration: 1,
},
legend: {
    delay: 1,
    duration: 1,
},
x: {
    delay: 1,
    duration: 1,
//                easing: 'ease-out'
}, 
y: {
    delay: 0,
    duration: 1,
//             easing: 'cubic-bezier(.39,0,.35,.99)'
}
}
),

    chart => chart.feature('tooltip',true)
];

export default testSteps;