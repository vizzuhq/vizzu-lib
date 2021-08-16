import { data } from '/test/integration/test_data/tutorial.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Timeseries'] },
          y: { attach: ['Values 1'], range: '0,1.1,%' },
          label: { attach: ['Values 1'] }
        },
        title: 'A simple column chart.',
        legend: null
      },
      style:
      { plot:
        { marker:
          { label:
            { position: 'top', filter: 'lightness(0)' }
          }
        } 
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { attach: ['Categ. Parent'] },
          color: { attach: ['Categ. Parent'] },
        },
        title: 'A cross-metric added to the y-axis and the color channel.',
        legend: "color"
      },
      style: {
        plot: {
          marker: {
            borderWidth: 1, borderOpacity: 1,
            label: { position: 'center', filter: 'lightness(0.6)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {},
        title: 'The geometric elements used can be changed to area...',
        geometry: 'area'
      },
      style:
      { plot:
        { marker:
          { label:
            { position: 'top', filter: 'lightness(0)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: '...or line...',
        geometry: 'line'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: '...or back to rectangle.',
        geometry: 'rectangle'
      },
      style:
      { plot:
        { marker:
          { label:
            { position: 'center', filter: 'lightness(0.6)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Values 1'] },
          x: { attach: ['Values 1'], range: '0,1.1,%' }
        },
        legend: null,
        title: 'Values can be grouped by the other axis too.',
      },
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          x: { detach: 'Timeseries' },
        },
        title: 'Values can be grouped by the other axis too.',
      },
      style:
      { plot:
        { marker:
          { label:
            { position: 'right', filter: 'lightness(0)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          x: { range: '0,1.333,%' },
        },
        title: 'Using polar coordinates instead of cartesian is also an option.',
        coordSystem: 'polar',
        legend: null
      },
      style: {
        plot: {
          marker: { label: { position: 'center', filter: 'lightness(0.6)' } },
          xAxis: { label: { color: '#ffffff00' } },
          yAxis: { label: { color: '#ffffff00' } }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          x: { attach: 'Timeseries' },
          lightness: { attach: ["Values 2"] }
        },
        title: 'Values can be added to the lightness channel.',
        legend: "lightness"
      },
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Categ. Parent'] },
          x: { detach: ['Values 1', 'Timeseries'] },
          size: { attach: ['Values 3', 'Timeseries'] }
        },
        title: 'Charts can also exist without any data on the axes.',
        legend: null,
        coordSystem: 'cartesian'
      },
      style: {
        plot: {
          xAxis: { label: { color: '#abababFF' } },
          yAxis: { label: { color: '#abababFF' } }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { attach: ["Categ. Child"] }
        },
        title: 'More categorical data series can be on the same channel.',
      },
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          size: { detach: ['Values 3', 'Timeseries', 'Categ. Child'] },
          y: { attach: ['Categ. Parent', 'Categ. Child', 'Values 1'], range: '0,1.1,%' },
          x: { attach: ['Timeseries'] },
          lightness: { detach: ['Values 2'] }
        },
        legend: "color",
        title: 'Getting back to the stacked bar chart in one step.'
      },
      style: {
        plot: {
          marker: {
            borderWidth: 1, borderOpacity: 0.6,
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Categ. Child'] },
        },
        title: 'Getting back to the stacked bar chart in one step.'
      },
      style: {
        plot: {
          marker: {
            borderWidth: 1, borderOpacity: 1,
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          y: { detach: ['Categ. Parent'] },
          x: { attach: ['Categ. Parent'] }
        },
        title: 'Data can be grouped...',
      },
      style: {
        plot: {
          marker: {
            borderWidth: 1, borderOpacity: 1,
            label: { position: 'top', filter: 'lightness(0)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        channels: {
          x: { detach: ['Categ. Parent'] },
          y: { attach: ['Categ. Parent'] }
        },
        title: '...or stacked, by putting a category from one axis to the other.',
      },
      style: {
        plot: {
          marker: {
            borderWidth: 0, borderOpacity: 1,
            label: { position: 'center', filter: 'lightness(0.6)' }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Comparing ratios is just another option out of many more...',
        align: 'stretch'
      },
    }
  )
];

export default testSteps;