import { data } from '/test/integration/test_data/chart_types_eu.js';

const testSteps = [
  chart => chart.animate(
    {
      data: data,
      descriptor: {
        channels: {
          x: { attach: ['Year'] },
          y: { attach: ['Value 5 (+/-)'], range: '0,1.1,%' },
          label: { attach: ['Value 5 (+/-)'] },
          size: { attach: ['Value 2 (+)'] }
        },
        title: 'Histogram with (-) Nums'
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