const testSteps = [
  (chart) => {
    const data = {
      series: [
        { name: 'Foo', values: ['a', 'b', 'c', 'a', 'b', 'c'] },
        { name: 'Foo2', values: ['A', 'A', 'A', 'B', 'B', 'B'] },
        { name: 'Bar', values: [150000000, 32, 12, 3, 2, 10], unit: 'p' }
      ]
    }
    return chart.animate({ data })
  },
  (chart) =>
    chart.animate({
      config: {
        size: 'count()',
        label: 'count()',
        geometry: 'circle',
        title: 'Pre title'
      }
    }),
  (chart) =>
    chart.animate({
      config: {
        legend: 'color',
        title: 'Some change'
      }
    })
]

export default testSteps
