const testSteps = [
  (chart) => chart.animate({
    data: {
      series: [
        {
          name: 'Year',
          type: 'dimension',
          values: ['2005', 'Total']
        },
        {
          name: 'Result',
          type: 'dimension',
          values: ['Won', 'Lost']
        }
      ]
    }
  }),
  (chart) => chart.animate({
    data: {
      filter: (record) => {
        return record.Year !== 'Total'
      }
    },
    config: {
      x: 'Year'
    }
  }),

  (chart) => chart.animate(
    {
      data: {
        filter: (record) => {
          return (
            record.Year === 'Total'
          )
        }
      },
      config: {
        y: 'Result'
      }
    }
  )
]

export default testSteps
