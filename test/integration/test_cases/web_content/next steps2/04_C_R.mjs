import { data_14 } from '../../../test_data/chart_types_eu.mjs';


const testSteps = [
    chart => chart.animate({
        data: data_14,
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' }
            },
            title: 'Stacked Column',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false,
            legend: 'color'
        }
    }
 ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Country','Year', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' }
            },
            title: 'Stacked Column / Aggregate(?), Sum(?), - Discrete(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false,
            legend: 'color'
        }
    },
       {
            duration: 2,
        }
    ),

    chart => chart.animate({
        config: {
            channels: {
                x: { set: null },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' },
                label: { set:['Value 2 (+)']}
            },
            title: 'Stacked Column / Aggregate(?), Sum(?), - Discrete(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false
        }
    },
    {
         delay: 0, duration: 0.5,
     }
 ),
 chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: ['Country','Year', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
            color: { set: 'Country' },
            label: { set:null }
            
        },
        title: 'Stacked Column / Drill down(?), + Discrete(?)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: 'color'
    }
},
   {
       delay:1, duration: 2,
    }
),

 chart => chart.animate({
        config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' }
            },
            title: 'Stacked Column',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false,
            legend: 'color'
        }
    }
 ),
 chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Value 2 (+)', 'Year'] },
            y: { set: ['Country'], range: { min: '0%', max: '100%' } },
            color: { set: null }
        },
        title: 'Bar / Aggregate(?), Sum(?)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: null
    }
}
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Value 2 (+)'] },
            y: { set: ['Country'] },
            color: { set: null },
            label: { set: ['Value 2 (+)']}
        },
        title: 'Bar / Aggregate(?), Sum(?)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: null
    }
},
   {
    delay: 0, duration: 0.4,
    }
),
chart => chart.animate({
    config: {
        channels: {
            x: { set: ['Value 2 (+)', 'Year'] },
            y: { set: ['Country'] },
            color: { set: null },
            label: { set: null }
        },
        title: 'Bar / Aggregate(?), Sum(?)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: null
    }
},
   {
        delay: 2, duration: 0.4
    }
),
chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
            color: { set: 'Country' }
        },
        title: 'Stacked Column / Drill down(?), + Discrete(?)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: 'color'
    }
}
),
chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: ['Value 2 (+)', 'Country', 'Year'] },
            color: { set: ['Country'] },
            label: { set: null}
        },
        title: 'Column / Total(?), Aggregate',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: 'color'
    }
},
{
    delay: 0, duration: 1,
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: ['Value 2 (+)', 'Country'] },
            color: { set: ['Country'] }
        },
        title: 'Column / Total(?), Aggregate',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: 'color'
    }
},
{
    delay: 0, duration: 0.3,
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: ['Value 2 (+)'] },
            color: { set: null },
            label: { set: ['Value 2 (+)']}
        },
        title: 'Column / Total(?), Aggregate',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: null
    }
},
{
    delay: 0, duration: 1
 }
),

chart => chart.animate({
    config: {
        channels: {
            x: { set: null },
            y: { set: ['Value 2 (+)', 'Country', 'Year'] },
            color: { set: null },
            label: { set: null}
        },
        title: 'Column / Sum(?)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: null
    }
},
{
    delay: 2, duration: 1,
 }
),
chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
            color: { set: 'Country' }
        },
        title: 'Stacked Column / Drill down(?), + Discrete(?)',
        geometry: 'rectangle',
        orientation: 'horizontal',
        split: false,
        legend: 'color'
    }
},
{
    delay: 0, duration: 2,
 }
),

chart => chart.animate({
    config: {
       channels: {
           x: { set: ['Year', 'Country'] },
           y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' }  },
           color: { set: 'Country' }
       },
       title: 'Groupped Column / Comparison(?), Group(?)',
       geometry: 'rectangle',
       orientation: 'horizontal',
       split: false,
       legend: null
   }
},
{
    delay: 2, duration: 1,
 }
),

 chart => chart.animate({
         config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' }
            },
            title: 'Stacked Column / Sum(?), Stack(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false,
            legend: 'color'
        }
    },
    {
     delay: 1
     }
 ),

 chart => chart.animate({
         config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' }
            },
            title: 'Splitted Column / Components(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: true,
            legend: 'color'
        }
    }
 ),

 chart => chart.animate({
         config: {
            channels: {
                x: { set: 'Year' },
                y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
                color: { set: 'Country' }
            },
            title: 'Stacked Column / Sum(?), Stack(?)',
            geometry: 'rectangle',
            orientation: 'horizontal',
            split: false,
            legend: 'color'
        }
    },
    {
     delay: 1
     }
 ),

 chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '100%' } },
            color: { set: 'Country' }
        },
        title: 'Stacked Percentage Column / Ratio%(?)',
        align: 'stretch'
    }
}),


chart => chart.animate({
    config: {
       channels: {
           x: { set: 'Year' },
           y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
           color: { set: 'Country' }
       },
       title: 'Stacked Column / Sum(?), Stack(?)',
       geometry: 'rectangle',
       orientation: 'horizontal',
       split: false,
       legend: 'color',
       align: 'min'
   }
},
{
 delay: 1
 }
),
chart => chart.animate({
    config: {
       channels: {
           x: { set: 'Value 5 (+/-)' },
           y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' }  },
           color: { set: 'Country' },
           noop: { set:'Year' }
       },
       title: 'Scatter plot / + Continuous(?)',
       geometry: 'circle',
       orientation: 'horizontal',
       split: false,
       legend: 'color',
       align: 'min'
   }
}
),
chart => chart.animate({
    config: {
       channels: {
           x: { set: 'Year' },
           y: { set: ['Country', 'Value 2 (+)'], range: { min: '0%', max: '110%' }  },
           color: { set: 'Country' },
           noop: { set: null }
       },
       title: 'Stacked Column / - Continuous(?)',
       geometry: 'rectangle',
       orientation: 'horizontal',
       split: false,
       legend: 'color',
       align: 'min'
   }
}
),


chart => chart.animate({
    config: {
        channels: {
            x: { set: 'Year' },
            y: { set: 'Value 2 (+)', range: { min: '0%', max: '110%' }  },
            color: { set: 'Country' }
        },
        title: 'Lollipop / Distribute(?)',
        geometry: 'circle',
        orientation: 'horizontal',
        split: false
    }
}
),

    chart => chart.feature('tooltip',true)
];

export default testSteps;