const dataLoaded = import('../assets/data/music_data.js')
const mdChartLoaded = import('../assets/javascripts/mdchart.js')

Promise.all([dataLoaded, mdChartLoaded]).then((results) => {
  const data = results[0].default
  const MdChart = results[1].default
  const mdchart = new MdChart(data, 'tutorial')

  mdchart.create([
    {
      anims: [
        (chart) => {
          return chart.animate({
            config: {
              title: 'Geometry: area',
              channels: {
                y: { set: 'Popularity' },
                x: { set: 'Genres' }
              }
            }
          })
        },
        (chart) => {
          return chart.animate({
            geometry: 'area'
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            title: 'Geometry: line'
          })
        },
        (chart) => {
          return chart.animate({
            geometry: 'line'
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            title: 'Geometry: circle'
          })
        },
        (chart) => {
          return chart.animate({
            geometry: 'circle'
          })
        }
      ]
    },
    {
      anims: [
        (chart) => {
          return chart.animate({
            title: 'Geometry: rectangle - default'
          })
        },
        (chart) => {
          return chart.animate({
            geometry: 'rectangle'
          })
        }
      ]
    }
  ])
})
