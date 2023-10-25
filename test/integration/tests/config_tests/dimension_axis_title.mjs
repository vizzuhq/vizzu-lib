const testSteps = [
  (chart) => {
    const data = {
      series: [
        { name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
        { name: 'Bar', values: ['Happy', 'Happy', 'Sad'] },
        { name: 'Baz', values: [1, 2, 3] }
      ]
    }

    return chart.animate({ data })
  },
  (chart) =>
    chart.animate({
      x: { set: 'Foo' },
      y: { set: 'Bar', title: 'Feeling' },
      legend: 'x'
    }),
  (chart) =>
    chart.animate({
      x: { title: 'Who' },
      y: { title: null },
    }),
  (chart) =>
    chart.animate({
      x: { set: 'Baz' }
    })
]

export default testSteps
