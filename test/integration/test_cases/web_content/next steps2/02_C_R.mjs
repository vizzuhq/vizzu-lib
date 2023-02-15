import { data_14 } from '../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_14,
        config: {
            channels: {
                x: { set: 'Country' },
                y: { set: 'Value 2 (+)' },
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
                marker: {
                    label: {
                        fontSize: null
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
        xAxis: {
            label: {
                angle: '3.14'
            }
        }
}
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
}
}
),

    chart => chart.feature('tooltip',true)
];

export default testSteps;