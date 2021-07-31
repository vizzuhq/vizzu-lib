import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Year'] },
          y: { attach: ['Negative nums'], range: '0,1.1,%' },
          label: { attach: ['Negative nums'] },
          size: { attach: ['Positive nums'] }
        },
        title: 'Histogram with (-) Nums'
      },
      style: {
        plot: { 
          paddingLeft: -20,
          marker: { label: {
            backgroundColor: '#FFFFFF60',
            filter: 'lightness(-0.2)',
            position: 'top',
//            orientation: 'normal'
          }}
        },
        data: { circleMinRadius: 0.01, circleMaxRadius: 0.05, }
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Circle plot with (-) Nums',
        geometry: 'circle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Histogram with (-) Nums',
        geometry: 'rectangle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Area with (-) Nums',
        geometry: 'area'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Histogram with (-) Nums',
        geometry: 'rectangle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Line with (-) Nums',
        geometry: 'line'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Circle with (-) Nums',
        geometry: 'circle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Area with (-) Nums',
        geometry: 'area'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Circle with (-) Nums',
        geometry: 'circle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Line with (-) Nums',
        geometry: 'line'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Area with (-) Nums',
        geometry: 'area'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Circle with (-) Nums',
        geometry: 'circle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Area with (-) Nums',
        geometry: 'area'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Line with (-) Nums',
        geometry: 'line'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Circle with (-) Nums',
        geometry: 'circle'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Line with (-) Nums',
        geometry: 'line'
      }
    }
  ),
  chart => chart.animate(
    {
      descriptor: {
        title: 'Area with (-) Nums',
        geometry: 'area'
      }
    }
  )
];

export default testSteps;