import data from '/example/data/generator.js';
console.log(data);

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      config: {
        channels: {
          size: { attach: ['Cont2'] },
          label: { attach: ['Cont2'] }
        },
        title: '1 Amount of a countinuous series.'
      },
      style: 
      {
        plot: 
        {
          marker:
          {
            label: 
            {
              numberFormat: 'prefixed'
            }
          }
        }
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          size: { attach: ['Years'] }
        },
        title: '2 Amount of a countinuous series.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          size: { detach: ['Years', 'Cont2'] },
          x: { attach: ['Years'] },
          y: { attach: ['Cont2'], range: '0,1.1,%' }
        },
        title: '3 A simple column chart.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { attach: ['Continents'] },
          color: { attach: ['Continents'] },
        },
        title: '4 A cross-metric added to the y-axis and the color channel.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {},
        title: '5 The geometric elements used can be changed to area...',
        geometry: 'area'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { detach: ['Continents'] }
        },
        title: '6 ...or line...',
        geometry: 'line'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { attach: ['Continents'] }
        },
        title: '7 ...or back to rectangle.',
        geometry: 'rectangle'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { detach: ['Cont2'] },
          x: { attach: ['Cont2'], range: '0,1.1,%' }
        },
        title: '8 Values can be grouped by the other axis too.',
      },
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          x: { detach: 'Years' },
        },
        title: '9 Values can be grouped by the other axis too.',
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          x: { range: '0,1.333,%' },
          label: { detach: 'Cont2' }
        },
        title: '10 Using polar coordinates instead of cartesian is also an option.',
        coordSystem: 'polar'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          x: { attach: [/*'Years',*/ 'Country code'] },
          lightness: { attach: ['Cont1'] }
        },
        title: '11 Values can be added to the lightness channel.'
      },
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          size: { attach: ['Cont2'], detach: ['Years'] },
          y: { detach: ['Continents'] },
          x: { detach: ['Cont2', /*'Years',*/ 'Country code'] },
          label: { attach: ['Country code'] }
        },
        title: '12 Charts can also exist without any data on the axes.',
        coordSystem: 'cartesian'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          size: { attach: ['Country code'] }
        },
        title: '13 More categorical data series can be on the same channel.',
      },
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          size: { detach: ['Cont2', /*'Years',*/ 'Country code'] },
          y: { attach: ['Continents', 'Country code', 'Cont2'], range: '0,1.1,%' },
          x: { attach: ['Years'] },
          lightness: { detach: ['Cont1'] }
        },
        title: '14 Getting back to the stacked bar chart in one step.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { detach: ['Country code'] },
        },
        title: '15 Getting back to the stacked bar chart in one step.'
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          y: { detach: ['Continents'] },
          x: { attach: ['Continents'] }
        },
        title: '16 Data can be grouped...',
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        channels: {
          x: { detach: ['Continents'] },
          y: { attach: ['Continents'] }
        },
        title: '17 ...or stacked, by putting a category from one axis to the other.',
      }
    }
  ),
  chart => chart.animate(
    {
      config: {
        title: '18 Comparing ratios is just another option out of many more...',
        align: 'stretch'
      },
    }
  )
];

export default testSteps;