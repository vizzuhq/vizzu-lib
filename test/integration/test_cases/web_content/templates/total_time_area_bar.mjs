import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [

    chart => {
        chart.on("mouseon", (param) => {
          if (param.data.marker === undefined)
            chart.animate( { config : { tooltip: null }} );
          else
            chart.animate( { config : { tooltip: param.data.marker.id }} );
        });
        return chart; 
      },


    chart => chart.animate(
        {
            data: data,
            config:
            {
                channels:
                {
                    y: { set: ['Country', 'Value 2 (+)'], range: {min:'0%', max:'110%'} },
                    x: { set: ['Year'] },
                    color: { set: ['Country'] },
                },
                title: 'Time distribution',
                geometry: 'area'
            },
            style:
            {
                tooltip:
                {
                    layout: 'multiLine',
                    dropShadow: '5',
                    arrowSize: '8',
                    seriesName: 'Country',
                    borderRadius: '5'
                }
            }
        }
    ),
    chart => chart.animate(
        {
            config: 
            {
                channels:
                {
                    y: { range: {min:'0%', max:'100%'} }
                },
                split: true
            }
        },
        {delay: 0}
    ),
    chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    y: { detach: ['Value 2 (+)'] },
                    x: { set: ['Value 2 (+)','Year'] }
                },
                title: 'Total',
                geometry: 'rectangle',
                split: false
            }
        },
        {
            geometry: { delay: 0, duration: 1 },
            y: { delay: 0, duration: 1 },
            x: { delay: 0, duration: 1 }
        }
    ),
    chart => chart.animate(
        {
            config:
            {
                channels:
                {
                    x: { detach: ['Year'] },
                    label: { attach: ['Value 2 (+)'] }
                }
            }
        }
    )
];

export default testSteps;